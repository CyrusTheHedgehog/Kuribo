#define KURIBO_ENABLE_LOG

#include "system/memory.hxx"
#include <EASTL/array.h>
#include <EASTL/string.h>

#include "common.h"

#include "api/HostInterop.h"
#include "io/io.hxx"

#include "system/system.hxx"
#include <core/patch.hxx>

#include <LibKuribo/filesystem.hxx>
#include <io/io.hxx>

#include "GeckoJIT/engine/compiler.hpp"

#include <modules/SymbolManager.hxx>
#include <modules/kxer/Module.hxx>

// eastl::array<char, 1024 * 4 * 64> heap;
void comet_app_install(void* image, void* vaddr_load, uint32_t load_size) {
  KURIBO_SCOPED_LOG("Installing...");

  char* base_addr = (char*)0x8042EB00 + 16 * 1024;
  constexpr u32 size = 923'448 - 2048 - 16 * 1024;

#ifdef _WIN32
  static char GLOBAL_HEAP[size];
  base_addr = &GLOBAL_HEAP[0];
#endif

  kuribo::mem::Init(base_addr, size, nullptr, 0);
  // kuribo::mem::AddRegion((void*)0x8042EB00, 923448, false);



  kuribo::System::createSystem();
  kuribo::SymbolManager::initializeStaticInstance();
  kuribo::kxRegisterProcedure("OSReport", FFI_NAME(os_report));
  kuribo::kxRegisterProcedure("kxGeckoJitCompileCodes",
                              (u32)&kuribo::kxGeckoJitCompileCodes);
#if 0
  auto our_module =
      eastl::make_unique<kuribo::KamekModule>("Kuribo/TestModule.kmk");
  if (our_module->mData == nullptr) {
    KURIBO_PRINTF("[KURIBO] Failed to load module\n");
  } else {
  }
#endif
  typedef f32 (*kxU32toF32_t)(u32);
  u32 in = 50;
  kxU32toF32_t cvt = (kxU32toF32_t)kuribo::kxGetProcedure("kxConvertU32toF32");
  if (cvt == nullptr) {
    KURIBO_LOG("Cannot find function!\n");
  } else {
    f32 out = cvt(in);
    KURIBO_LOG("RESULT: %f\n", out);
  }
  {
    // auto our_module =
    //    eastl::make_unique<kuribo::KamekModule>("Kuribo/DebugMem.kmk");
    // if (our_module->mData == nullptr) {
    //  KURIBO_PRINTF("[KURIBO] Failed to load module\n");
    //} else {
    //  kuribo::System::getSystem().mProjectManager.attachModule(
    //      eastl::move(our_module));
    //}
    // kuribo::Critical q;
    int size, rsize;
    auto kxmodule = kuribo::io::dvd::loadFile("Kuribo/GCC.kxe", &size, &rsize);

    kuribo::KuriboModule module(kxmodule.get(), size,
                                &kuribo::mem::GetDefaultHeap());
    KURIBO_PRINTF("FINISHED\n");
  }
}