#include "module.h"
#include "kernel.h"

bool Module::Initialise(struct multiboot_info * MultiBootInfo) {

	//Get a reference to all modules and store for safe keeping
    if (MultiBootInfo->mods_count == 0) {
        message("Unable to boot -- No modules loaded");
        __asm__ __volatile__ ("hlt");
        return false;
    }

    if (items == NULL) {
        items = new warp::Vector<ModuleItem>();
    }

    message("Loading Kernel Modules * %lu\n", MultiBootInfo->mods_count);

    char extensionBuffer[5];

    for (unsigned long i=0; i<MultiBootInfo->mods_count; i++) {

        unsigned long offset = MultiBootInfo->mods_addr + (i * sizeof(struct mod_list));

        struct mod_list * module = (struct mod_list *)offset;

        //Check that it is a zip, by checking extension
        extensionBuffer[4] = 0;
        char * cmdline = (char *)module->cmdline;
        int len = strlen(cmdline);
        int counter = 0;
        for (int j = len - 4; j < len; j++) {
            extensionBuffer[counter] = cmdline[j];
            counter++;
        }

        my::String ext(extensionBuffer);

        if (ext.toUpper().compare(0, 4, ".ZIP") == 0) {
            message("Loading .zip Module: %s\n", cmdline);

            ModuleItem * item = new ModuleItem;

            item->base = new Zip();
            if (!item->base->LoadZip((unsigned char *)module->mod_start, (size_t)(module->mod_end - module->mod_start))) {
                delete item;
            } else {
                //Get filenames
                warp::Vector<my::String> * files = new warp::Vector<my::String>();

                if (!item->base->ListFiles(files)) {
                    delete item;
                    delete files;
                }

                size_t numFiles = files->size();

                //Not required -- good for faster lookups
                item->files = new warp::Vector<ZipItem>();

                for (size_t i=0; i<numFiles; i++) {

                    ZipItem * file = new ZipItem;

                    file->filename = files->pop();

                    file->filename->toUpper();

                    size_t len = file->filename->length();

                    ssize_t extOffset = file->filename->lastIndexOf('.');

                    if (extOffset != -1) {
                        file->extension = new my::String(file->filename->substr(extOffset, len));
                    } else {
                        file->extension = new my::String(* file->filename);
                        file->extension->toUpper();
                    }

                    item->files->push(file);
                }

                items->push(item);
            }

        } else {
            message("Ignoring %s Module: %s\n", extensionBuffer, cmdline);
        }

    }

    return true;
}

bool Module::ListFilesOfType(my::String extension, warp::Vector<ModuleFile> *vec) {

    if (items == NULL)
        return false;

    if (items->size() == 0)
        return false;

    extension.toUpper();

    size_t extLength = extension.length();

    size_t numModules = items->size();

    for (size_t i=0; i<numModules; i++) {

        size_t numFiles = items->at(i)->files->size();

        for (size_t j=0; j<numFiles; j++) {

            ZipItem * file = items->at(i)->files->at(j);

            if (extension.compare(0, extLength, * file->filename) == 0) {
                ModuleFile * modFile = new ModuleFile;

                modFile->ModuleId = i;
                modFile->Filename = * file->filename;

                vec->push(modFile);
            }

        }

    }

    return true;

}

unsigned char * Module::Open(int moduleId, my::String filename) {

    if (items == NULL)
        return false;

    if (moduleId > ((int)items->size() - 1))
        return false;

    filename.toUpper();

    ModuleItem * module = items->at(moduleId);

    //Find file with filename
    size_t numFiles = module->files->size();

    for (size_t i=0; i<numFiles; i++) {
        ZipItem * file = module->files->at(i);

        if (filename.compare(0, filename.length(), * file->filename) == 0) {

            //Get the data!
            return module->base->OpenFile(filename);

        }
    }

    return NULL;
}

bool Module::Exec(int moduleId, my::String filename) {

    unsigned char * exec = Open(moduleId, filename);

    message("%s\n", exec);

    return true;
}

void Module::Close(unsigned char * data) {

	dlfree(data);

}
