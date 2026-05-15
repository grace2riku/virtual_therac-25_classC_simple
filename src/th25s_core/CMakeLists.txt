# TH25S-CORE 安全コアライブラリ (構成アイテム CI-SRC-001)。
# UNIT-001 CommonTypes / UNIT-002 TreatmentSequencer / UNIT-003 SafetyInterlock。

add_library(th25s_core STATIC
    src/common_types.c
    src/treatment_sequencer.c
    src/safety_interlock.c
)

target_include_directories(th25s_core
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_options(th25s_core PRIVATE ${TH25S_C_WARNINGS})
