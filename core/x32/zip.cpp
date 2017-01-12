#include "zip.h"
#include "kernel.h"

bool Zip::LoadZip(unsigned char *data, size_t size) {

    if (zipArchive == NULL)
        zipArchive = new mz_zip_archive;

    memset(zipArchive, 0, sizeof(mz_zip_archive));

    mz_bool status = mz_zip_reader_init_mem(zipArchive, data, size, 0);

    if (!status) {
        message("Unable to read zip file: %s", mz_error(status));
        return false;
    }

    return true;
}

bool Zip::ListFiles(warp::Vector<my::String> *vec) {

    mz_uint numFiles = mz_zip_reader_get_num_files(zipArchive);

    message("Reading %i files\n", (int)numFiles);

    for (mz_uint i=0; i<numFiles; i++) {

        if (mz_zip_reader_get_filename(zipArchive, i, buf, 512)) {
            my::String * s = new my::String(buf);
            vec->push(s);
        }

    }

    return true;
}

unsigned char * Zip::OpenFile(my::String filename) {

    mz_zip_archive_file_stat stat;

    mz_uint numFiles = mz_zip_reader_get_num_files(zipArchive);

    for (mz_uint i=0; i<numFiles; i++) {

        if (mz_zip_reader_get_filename(zipArchive, i, buf, 512)) {

            my::String * s = new my::String(buf);
            s->toUpper();

            if (filename.toUpper().compare(0, filename.length(), * s) == 0) {

                if (mz_zip_reader_file_stat(zipArchive, i, &stat)) {

                    unsigned char * data = (unsigned char *)dlmalloc((size_t)stat.m_uncomp_size);

                    if (mz_zip_reader_extract_file_to_mem(zipArchive, filename.c_str(), data, (size_t)stat.m_uncomp_size, 0)) {
                        return data;
                    }
                }
            }
        }

    }

    return NULL;

}
