/*
BSD 2-Clause License

Copyright (c) 2020, Firstbober
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef OBJECTIFIER_H
#define OBJECTIFIER_H

#include "uthash.h"
#include <stdbool.h>

typedef struct {
  void *_struct;
  void (*destructor)();
  bool dynamic;
  const char *source_func;
} OCR_Master_STContainer;

typedef struct {
  const char *identifier;
  OCR_Master_STContainer container;
  UT_hash_handle hh;
} OCR_Master;

OCR_Master *OCR_Manager = NULL;

void OCR_add_struct(char *identifier, void *st, void *destructor, bool dynamic,
                    const char *source) {
  OCR_Master *storage;

  storage = (OCR_Master *)malloc(sizeof(OCR_Master));
  storage->identifier = identifier;
  storage->container = (OCR_Master_STContainer){._struct = st,
                                                .destructor = destructor,
                                                .dynamic = dynamic,
                                                .source_func = source};
  HASH_ADD_STR(OCR_Manager, identifier, storage);
}

#define OCR_init_internal(st, dyn, ...)                                        \
  ({                                                                           \
    st *_struct = malloc(sizeof(st));                                          \
    OCR_add_struct((void *)_struct, _struct, st##_destructor, dyn, __func__);  \
    st##_constructor(_struct, ##__VA_ARGS__);                                  \
    _struct;                                                                   \
  })

#define OCR_init(st, ...) OCR_init_internal(st, false, ##__VA_ARGS__)
#define OCR_init_dyn(st, ...) OCR_init_internal(st, true, ##__VA_ARGS__)

#define OCR_method(type, name, ...) type (*name)(void *this, ##__VA_ARGS__)
#define OCR_memberize(st, st_name, name) st->name = (void *)st_name##_##name;
#define OCR_call(st, name, ...) st->name(st, ##__VA_ARGS__)

void OCR_free_all() {
  OCR_Master *storage;
  for (storage = OCR_Manager; storage != NULL;
       storage = (OCR_Master *)storage->hh.next) {

    if (storage->container.destructor != NULL) {
      storage->container.destructor();
    }

    free(storage->container._struct);
    HASH_DEL(OCR_Manager, storage);
    free(storage);
  }
}

void OCR_free(void *st) {
  OCR_Master *storage;
  for (storage = OCR_Manager; storage != NULL;
       storage = (OCR_Master *)storage->hh.next) {

    if (storage->container._struct == st) {
      if (storage->container.destructor != NULL) {
        storage->container.destructor();
      }

      free(storage->container._struct);
      HASH_DEL(OCR_Manager, storage);
      free(storage);
    }
  }
}

void OCR_dyn_clear(const char *source, void *returned) {
  if (strcmp(source, "main") == 0) {
    OCR_free_all();
  } else {
    OCR_Master *storage;
    for (storage = OCR_Manager; storage != NULL;
         storage = (OCR_Master *)storage->hh.next) {

      if (storage->container.dynamic == true &&
          storage->container._struct != returned &&
          storage->container.source_func == source) {
        printf("deleted in: %s\n", source);
        if (storage->container.destructor != NULL) {
          storage->container.destructor();
        }
        free(storage->container._struct);
        HASH_DEL(OCR_Manager, storage);
        free(storage);
      }
    }
  }
}

#define OCR_class(name)                                                        \
  typedef struct name name;                                                    \
  struct name

#define OCR_return(r)                                                          \
  OCR_dyn_clear(__func__, r);                                                  \
  return r;

#ifdef OCR_SIMPLY_NAMES

#define class OCR_class
#define method OCR_method
#define memberize OCR_memberize
#define init OCR_init
#define init_dyn OCR_init_dyn
#define call OCR_call
#define return(r) OCR_return(r)

#endif

#endif /* OBJECTIFIER_H */
