/*-*- mode:c;indent-tabs-mode:nil;c-basic-offset:2;tab-width:8;coding:utf-8 -*-│
│vi: set net ft=c ts=2 sts=2 sw=2 fenc=utf-8                                :vi│
╞══════════════════════════════════════════════════════════════════════════════╡
│ Copyright 2022 Justine Alexandra Roberts Tunney                              │
│                                                                              │
│ Permission to use, copy, modify, and/or distribute this software for         │
│ any purpose with or without fee is hereby granted, provided that the         │
│ above copyright notice and this permission notice appear in all copies.      │
│                                                                              │
│ THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL                │
│ WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED                │
│ WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE             │
│ AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL         │
│ DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR        │
│ PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER               │
│ TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR             │
│ PERFORMANCE OF THIS SOFTWARE.                                                │
╚─────────────────────────────────────────────────────────────────────────────*/
#include "libc/intrin/atomic.h"
#include "libc/intrin/dll.h"
#include "libc/runtime/runtime.h"
#include "libc/thread/posixthread.internal.h"
#include "libc/thread/thread.h"
#include "libc/thread/tls.h"

/**
 * Releases memory of detached threads that have terminated.
 */
void pthread_decimate_np(void) {
  struct Dll *e;
  struct PosixThread *pt;
  enum PosixThreadStatus status;
StartOver:
  pthread_spin_lock(&_pthread_lock);
  for (e = dll_first(_pthread_list); e; e = dll_next(_pthread_list, e)) {
    pt = POSIXTHREAD_CONTAINER(e);
    if (pt->tib == __get_tls()) continue;
    status = atomic_load_explicit(&pt->status, memory_order_acquire);
    if (status != kPosixThreadZombie) break;
    if (!atomic_load_explicit(&pt->tib->tib_tid, memory_order_acquire)) {
      dll_remove(&_pthread_list, e);
      pthread_spin_unlock(&_pthread_lock);
      _pthread_free(pt);
      goto StartOver;
    }
  }
  pthread_spin_unlock(&_pthread_lock);
}
