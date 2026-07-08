#ifndef XXX_ASSERT_H
#define XXX_ASSERT_H

#ifndef XXX_ASSERT
#  include <stdio.h>
#  include <stdlib.h>
#  define XXX_ASSERT(condition, format, ...)                                                                \
    do {                                                                                                    \
        if (!(condition)) {                                                                                 \
            fprintf(stderr, "%s:%d: %s(): " format "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__);      \
            abort();                                                                                        \
        }                                                                                                   \
    } while (0)
#endif

#endif
