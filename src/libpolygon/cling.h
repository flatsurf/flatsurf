#ifndef LIBPOLYGON_CLING_H
#define LIBPOLYGON_CLING_H

#ifdef __CLING__

#pragma cling add_library_path("/usr/local/lib")
#pragma cling load("libpolygon")

#endif // __CLING__

#endif // LIBPOLYGON_CLING_H
