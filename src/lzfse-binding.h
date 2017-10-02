#ifndef _NODE_lzfse_H_
#define _NODE_lzfse_H_

#include <node.h>
#include <v8.h>
#include "nan.h"

namespace nodelzfse {

class CompressBinding {
 public:
    static NAN_METHOD(Sync);
};

class DecompressBinding {
 public:
    static NAN_METHOD(Sync);
};

}

#endif
