
# Consider dependencies only in project.
set(CMAKE_DEPENDS_IN_PROJECT_ONLY OFF)

# The set of languages for which implicit dependencies are needed:
set(CMAKE_DEPENDS_LANGUAGES
  "ASM"
  )
# The set of files for implicit dependencies of each language:
set(CMAKE_DEPENDS_CHECK_ASM
  "/home/plebish/pico-sdk/src/rp2_common/hardware_irq/irq_handler_chain.S" "/home/plebish/pockpet_v2/build/lib/FreeRTOS_Kernel/portable/CMakeFiles/freertos_kernel_port.dir/home/plebish/pico-sdk/src/rp2_common/hardware_irq/irq_handler_chain.S.obj"
  )

# Preprocessor definitions for this target.
set(CMAKE_TARGET_DEFINITIONS_ASM
  "LIB_PICO_PLATFORM=1"
  "LIB_PICO_SYNC=1"
  "LIB_PICO_SYNC_CRITICAL_SECTION=1"
  "LIB_PICO_SYNC_MUTEX=1"
  "LIB_PICO_SYNC_SEM=1"
  "LIB_PICO_TIME=1"
  "LIB_PICO_UTIL=1"
  "PICO_BOARD=\"pico\""
  "PICO_BUILD=1"
  "PICO_NO_HARDWARE=0"
  "PICO_ON_DEVICE=1"
  )

# The include file search paths:
set(CMAKE_ASM_TARGET_INCLUDE_PATH
  "../lib/FreeRTOS_Kernel/portable/ThirdParty/GCC/RP2040/include"
  "/home/plebish/pico-sdk/src/common/pico_base/include"
  "generated/pico_base"
  "/home/plebish/pico-sdk/src/boards/include"
  "/home/plebish/pico-sdk/src/rp2_common/pico_platform/include"
  "/home/plebish/pico-sdk/src/rp2040/hardware_regs/include"
  "/home/plebish/pico-sdk/src/rp2_common/hardware_base/include"
  "../lib/FreeRTOS_Kernel/include"
  "../src"
  "/home/plebish/pico-sdk/src/rp2_common/hardware_clocks/include"
  "/home/plebish/pico-sdk/src/rp2040/hardware_structs/include"
  "/home/plebish/pico-sdk/src/rp2_common/hardware_claim/include"
  "/home/plebish/pico-sdk/src/rp2_common/hardware_sync/include"
  "/home/plebish/pico-sdk/src/rp2_common/hardware_gpio/include"
  "/home/plebish/pico-sdk/src/rp2_common/hardware_irq/include"
  "/home/plebish/pico-sdk/src/common/pico_sync/include"
  "/home/plebish/pico-sdk/src/common/pico_time/include"
  "/home/plebish/pico-sdk/src/rp2_common/hardware_timer/include"
  "/home/plebish/pico-sdk/src/common/pico_util/include"
  "/home/plebish/pico-sdk/src/rp2_common/hardware_resets/include"
  "/home/plebish/pico-sdk/src/rp2_common/hardware_pll/include"
  "/home/plebish/pico-sdk/src/rp2_common/hardware_vreg/include"
  "/home/plebish/pico-sdk/src/rp2_common/hardware_watchdog/include"
  "/home/plebish/pico-sdk/src/rp2_common/hardware_xosc/include"
  "/home/plebish/pico-sdk/src/rp2_common/hardware_exception/include"
  )

# The set of dependency files which are needed:
set(CMAKE_DEPENDS_DEPENDENCY_FILES
  "/home/plebish/pockpet_v2/lib/FreeRTOS_Kernel/portable/ThirdParty/GCC/RP2040/idle_task_static_memory.c" "lib/FreeRTOS_Kernel/portable/CMakeFiles/freertos_kernel_port.dir/ThirdParty/GCC/RP2040/idle_task_static_memory.c.obj" "gcc" "lib/FreeRTOS_Kernel/portable/CMakeFiles/freertos_kernel_port.dir/ThirdParty/GCC/RP2040/idle_task_static_memory.c.obj.d"
  "/home/plebish/pockpet_v2/lib/FreeRTOS_Kernel/portable/ThirdParty/GCC/RP2040/port.c" "lib/FreeRTOS_Kernel/portable/CMakeFiles/freertos_kernel_port.dir/ThirdParty/GCC/RP2040/port.c.obj" "gcc" "lib/FreeRTOS_Kernel/portable/CMakeFiles/freertos_kernel_port.dir/ThirdParty/GCC/RP2040/port.c.obj.d"
  "/home/plebish/pico-sdk/src/common/pico_sync/critical_section.c" "lib/FreeRTOS_Kernel/portable/CMakeFiles/freertos_kernel_port.dir/home/plebish/pico-sdk/src/common/pico_sync/critical_section.c.obj" "gcc" "lib/FreeRTOS_Kernel/portable/CMakeFiles/freertos_kernel_port.dir/home/plebish/pico-sdk/src/common/pico_sync/critical_section.c.obj.d"
  "/home/plebish/pico-sdk/src/common/pico_sync/lock_core.c" "lib/FreeRTOS_Kernel/portable/CMakeFiles/freertos_kernel_port.dir/home/plebish/pico-sdk/src/common/pico_sync/lock_core.c.obj" "gcc" "lib/FreeRTOS_Kernel/portable/CMakeFiles/freertos_kernel_port.dir/home/plebish/pico-sdk/src/common/pico_sync/lock_core.c.obj.d"
  "/home/plebish/pico-sdk/src/common/pico_sync/mutex.c" "lib/FreeRTOS_Kernel/portable/CMakeFiles/freertos_kernel_port.dir/home/plebish/pico-sdk/src/common/pico_sync/mutex.c.obj" "gcc" "lib/FreeRTOS_Kernel/portable/CMakeFiles/freertos_kernel_port.dir/home/plebish/pico-sdk/src/common/pico_sync/mutex.c.obj.d"
  "/home/plebish/pico-sdk/src/common/pico_sync/sem.c" "lib/FreeRTOS_Kernel/portable/CMakeFiles/freertos_kernel_port.dir/home/plebish/pico-sdk/src/common/pico_sync/sem.c.obj" "gcc" "lib/FreeRTOS_Kernel/portable/CMakeFiles/freertos_kernel_port.dir/home/plebish/pico-sdk/src/common/pico_sync/sem.c.obj.d"
  "/home/plebish/pico-sdk/src/common/pico_time/time.c" "lib/FreeRTOS_Kernel/portable/CMakeFiles/freertos_kernel_port.dir/home/plebish/pico-sdk/src/common/pico_time/time.c.obj" "gcc" "lib/FreeRTOS_Kernel/portable/CMakeFiles/freertos_kernel_port.dir/home/plebish/pico-sdk/src/common/pico_time/time.c.obj.d"
  "/home/plebish/pico-sdk/src/common/pico_time/timeout_helper.c" "lib/FreeRTOS_Kernel/portable/CMakeFiles/freertos_kernel_port.dir/home/plebish/pico-sdk/src/common/pico_time/timeout_helper.c.obj" "gcc" "lib/FreeRTOS_Kernel/portable/CMakeFiles/freertos_kernel_port.dir/home/plebish/pico-sdk/src/common/pico_time/timeout_helper.c.obj.d"
  "/home/plebish/pico-sdk/src/common/pico_util/datetime.c" "lib/FreeRTOS_Kernel/portable/CMakeFiles/freertos_kernel_port.dir/home/plebish/pico-sdk/src/common/pico_util/datetime.c.obj" "gcc" "lib/FreeRTOS_Kernel/portable/CMakeFiles/freertos_kernel_port.dir/home/plebish/pico-sdk/src/common/pico_util/datetime.c.obj.d"
  "/home/plebish/pico-sdk/src/common/pico_util/pheap.c" "lib/FreeRTOS_Kernel/portable/CMakeFiles/freertos_kernel_port.dir/home/plebish/pico-sdk/src/common/pico_util/pheap.c.obj" "gcc" "lib/FreeRTOS_Kernel/portable/CMakeFiles/freertos_kernel_port.dir/home/plebish/pico-sdk/src/common/pico_util/pheap.c.obj.d"
  "/home/plebish/pico-sdk/src/common/pico_util/queue.c" "lib/FreeRTOS_Kernel/portable/CMakeFiles/freertos_kernel_port.dir/home/plebish/pico-sdk/src/common/pico_util/queue.c.obj" "gcc" "lib/FreeRTOS_Kernel/portable/CMakeFiles/freertos_kernel_port.dir/home/plebish/pico-sdk/src/common/pico_util/queue.c.obj.d"
  "/home/plebish/pico-sdk/src/rp2_common/hardware_claim/claim.c" "lib/FreeRTOS_Kernel/portable/CMakeFiles/freertos_kernel_port.dir/home/plebish/pico-sdk/src/rp2_common/hardware_claim/claim.c.obj" "gcc" "lib/FreeRTOS_Kernel/portable/CMakeFiles/freertos_kernel_port.dir/home/plebish/pico-sdk/src/rp2_common/hardware_claim/claim.c.obj.d"
  "/home/plebish/pico-sdk/src/rp2_common/hardware_clocks/clocks.c" "lib/FreeRTOS_Kernel/portable/CMakeFiles/freertos_kernel_port.dir/home/plebish/pico-sdk/src/rp2_common/hardware_clocks/clocks.c.obj" "gcc" "lib/FreeRTOS_Kernel/portable/CMakeFiles/freertos_kernel_port.dir/home/plebish/pico-sdk/src/rp2_common/hardware_clocks/clocks.c.obj.d"
  "/home/plebish/pico-sdk/src/rp2_common/hardware_exception/exception.c" "lib/FreeRTOS_Kernel/portable/CMakeFiles/freertos_kernel_port.dir/home/plebish/pico-sdk/src/rp2_common/hardware_exception/exception.c.obj" "gcc" "lib/FreeRTOS_Kernel/portable/CMakeFiles/freertos_kernel_port.dir/home/plebish/pico-sdk/src/rp2_common/hardware_exception/exception.c.obj.d"
  "/home/plebish/pico-sdk/src/rp2_common/hardware_gpio/gpio.c" "lib/FreeRTOS_Kernel/portable/CMakeFiles/freertos_kernel_port.dir/home/plebish/pico-sdk/src/rp2_common/hardware_gpio/gpio.c.obj" "gcc" "lib/FreeRTOS_Kernel/portable/CMakeFiles/freertos_kernel_port.dir/home/plebish/pico-sdk/src/rp2_common/hardware_gpio/gpio.c.obj.d"
  "/home/plebish/pico-sdk/src/rp2_common/hardware_irq/irq.c" "lib/FreeRTOS_Kernel/portable/CMakeFiles/freertos_kernel_port.dir/home/plebish/pico-sdk/src/rp2_common/hardware_irq/irq.c.obj" "gcc" "lib/FreeRTOS_Kernel/portable/CMakeFiles/freertos_kernel_port.dir/home/plebish/pico-sdk/src/rp2_common/hardware_irq/irq.c.obj.d"
  "/home/plebish/pico-sdk/src/rp2_common/hardware_pll/pll.c" "lib/FreeRTOS_Kernel/portable/CMakeFiles/freertos_kernel_port.dir/home/plebish/pico-sdk/src/rp2_common/hardware_pll/pll.c.obj" "gcc" "lib/FreeRTOS_Kernel/portable/CMakeFiles/freertos_kernel_port.dir/home/plebish/pico-sdk/src/rp2_common/hardware_pll/pll.c.obj.d"
  "/home/plebish/pico-sdk/src/rp2_common/hardware_sync/sync.c" "lib/FreeRTOS_Kernel/portable/CMakeFiles/freertos_kernel_port.dir/home/plebish/pico-sdk/src/rp2_common/hardware_sync/sync.c.obj" "gcc" "lib/FreeRTOS_Kernel/portable/CMakeFiles/freertos_kernel_port.dir/home/plebish/pico-sdk/src/rp2_common/hardware_sync/sync.c.obj.d"
  "/home/plebish/pico-sdk/src/rp2_common/hardware_timer/timer.c" "lib/FreeRTOS_Kernel/portable/CMakeFiles/freertos_kernel_port.dir/home/plebish/pico-sdk/src/rp2_common/hardware_timer/timer.c.obj" "gcc" "lib/FreeRTOS_Kernel/portable/CMakeFiles/freertos_kernel_port.dir/home/plebish/pico-sdk/src/rp2_common/hardware_timer/timer.c.obj.d"
  "/home/plebish/pico-sdk/src/rp2_common/hardware_vreg/vreg.c" "lib/FreeRTOS_Kernel/portable/CMakeFiles/freertos_kernel_port.dir/home/plebish/pico-sdk/src/rp2_common/hardware_vreg/vreg.c.obj" "gcc" "lib/FreeRTOS_Kernel/portable/CMakeFiles/freertos_kernel_port.dir/home/plebish/pico-sdk/src/rp2_common/hardware_vreg/vreg.c.obj.d"
  "/home/plebish/pico-sdk/src/rp2_common/hardware_watchdog/watchdog.c" "lib/FreeRTOS_Kernel/portable/CMakeFiles/freertos_kernel_port.dir/home/plebish/pico-sdk/src/rp2_common/hardware_watchdog/watchdog.c.obj" "gcc" "lib/FreeRTOS_Kernel/portable/CMakeFiles/freertos_kernel_port.dir/home/plebish/pico-sdk/src/rp2_common/hardware_watchdog/watchdog.c.obj.d"
  "/home/plebish/pico-sdk/src/rp2_common/hardware_xosc/xosc.c" "lib/FreeRTOS_Kernel/portable/CMakeFiles/freertos_kernel_port.dir/home/plebish/pico-sdk/src/rp2_common/hardware_xosc/xosc.c.obj" "gcc" "lib/FreeRTOS_Kernel/portable/CMakeFiles/freertos_kernel_port.dir/home/plebish/pico-sdk/src/rp2_common/hardware_xosc/xosc.c.obj.d"
  "/home/plebish/pico-sdk/src/rp2_common/pico_platform/platform.c" "lib/FreeRTOS_Kernel/portable/CMakeFiles/freertos_kernel_port.dir/home/plebish/pico-sdk/src/rp2_common/pico_platform/platform.c.obj" "gcc" "lib/FreeRTOS_Kernel/portable/CMakeFiles/freertos_kernel_port.dir/home/plebish/pico-sdk/src/rp2_common/pico_platform/platform.c.obj.d"
  )

# Targets to which this target links.
set(CMAKE_TARGET_LINKED_INFO_FILES
  "/home/plebish/pockpet_v2/build/lib/FreeRTOS_Kernel/CMakeFiles/freertos_kernel.dir/DependInfo.cmake"
  )

# Fortran module output directory.
set(CMAKE_Fortran_TARGET_MODULE_DIR "")
