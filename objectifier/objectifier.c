#include "objecitifier.h"
#include "uthash.h"

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