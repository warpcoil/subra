#ifndef __ZIP_H__
#define __ZIP_H__

#include "miniz/miniz.h"
#include "my/my_string.h"
#include "warp/vector.h"

class Zip {
private:
    mz_zip_archive * zipArchive = NULL;
    char buf[512];
public:
    bool LoadZip(unsigned char * data, size_t size);
    bool ListFiles(warp::Vector<my::String> * vec);
    char * OpenFile(my::String filename);

};

#endif // !__ZIP_H__
