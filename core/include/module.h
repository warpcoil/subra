#ifndef __MODULE_H__
#define __MODULE_H__

#include "warp/vector.h"
#include "oslib/mb-info.h"
#include "zip.h"

class Module {
private:
	typedef struct ZipItem {
        my::String * filename;
        my::String * extension;
	} ZipItem;

	typedef struct ModuleItem {
		Zip * base;
        warp::Vector<ZipItem> * files;
	} ModuleItem;

    warp::Vector<ModuleItem> * items = NULL;
public:
    typedef struct ModuleFile {
        int ModuleId;
        my::String Filename;
    } ModuleFile;

	bool Initialise(struct multiboot_info * MultiBootInfo);
    bool ListFilesOfType(my::String extension, warp::Vector<ModuleFile> * vec);
    unsigned char * Open(int moduleId, my::String filename);
    bool Exec(int moduleId, my::String filename);
    void Close(unsigned char * data);
};

#endif
