/*********************************************************************
  Blosc - Blocked Shuffling and Compression Library

  Author: The Blosc Developers <blosc@blosc.org>

  See LICENSE.txt for details about copyright and rights to use.
**********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "blosc2.h"
#include "blosc-private.h"
#include "frame.h"

#if defined(_WIN32) && !defined(__MINGW32__)
#include <windows.h>
  #include <malloc.h>

/* stdint.h only available in VS2010 (VC++ 16.0) and newer */
  #if defined(_MSC_VER) && _MSC_VER < 1600
    #include "win32/stdint-windows.h"
  #else
    #include <stdint.h>
  #endif

  #define fseek _fseeki64

#endif  /* _WIN32 */

/* If C11 is supported, use it's built-in aligned allocation. */
#if __STDC_VERSION__ >= 201112L
#include <stdalign.h>
#endif


/* Open sparse frame index chunk */
void* sframe_open_index(const char* urlpath, const char* mode, const blosc2_io *io) {
  void* fp = NULL;
  char* index_path = malloc(strlen(urlpath) + strlen("/chunks.b2frame") + 1);
  if (index_path) {
    sprintf(index_path, "%s/chunks.b2frame", urlpath);
    fp = io->open(index_path, mode, io->params);
    free(index_path);
  }
  return fp;
}

/* Open directory/nchunk.chunk with 8 zeros of padding */
void* sframe_open_chunk(const char* urlpath, int64_t nchunk, const char* mode, const blosc2_io *io) {
  void* fp = NULL;
  char* chunk_path = malloc(strlen(urlpath) + 1 + 8 + strlen(".chunk") + 1);
  if (chunk_path) {
    sprintf(chunk_path, "%s/%08X.chunk", urlpath, (unsigned int)nchunk);
    fp = io->open(chunk_path, mode, io->params);
    free(chunk_path);
  }
  return fp;
}

/* Append an existing chunk into a sparse frame. */
void* sframe_create_chunk(blosc2_frame_s* frame, uint8_t* chunk, int32_t nchunk, int64_t cbytes) {
  void* fpc = sframe_open_chunk(frame->urlpath, nchunk, "wb", frame->schunk->storage->udio);
  if (fpc == NULL) {
    BLOSC_TRACE_ERROR("Cannot open the chunkfile.");
    return NULL;
  }
  blosc2_io *io = frame->schunk->storage->udio;
  size_t wbytes = io->write(chunk, 1, cbytes, fpc, io->params);
  io->close(fpc, io->params);
  if (wbytes != (size_t)cbytes) {
    BLOSC_TRACE_ERROR("Cannot write the full chunk.");
    return NULL;
  }

  return frame;
}


/* Get chunk from sparse frame. */
int sframe_get_chunk(blosc2_frame_s* frame, int32_t nchunk, uint8_t** chunk, bool* needs_free){
  void *fpc = sframe_open_chunk(frame->urlpath, nchunk, "rb", frame->schunk->storage->udio);
  if(fpc == NULL){
    BLOSC_TRACE_ERROR("Cannot open the chunkfile.");
    return BLOSC2_ERROR_FILE_OPEN;
  }

  blosc2_io *io = frame->schunk->storage->udio;
  io->seek(fpc, 0L, SEEK_END, io->params);
  int32_t chunk_cbytes = ftell(fpc);
  *chunk = malloc((size_t)chunk_cbytes);

  io->seek(fpc, 0L, SEEK_SET, io->params);
  size_t rbytes = io->read(*chunk, 1, (size_t)chunk_cbytes, fpc, io->params);
  io->close(fpc, io->params);
  if (rbytes != (size_t)chunk_cbytes) {
    BLOSC_TRACE_ERROR("Cannot read the chunk out of the chunkfile.");
    return BLOSC2_ERROR_FILE_READ;
  }
  *needs_free = true;

  return chunk_cbytes;
}
