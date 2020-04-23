project(modEQ)

if(APPLE)
        set(TOBANTEAUDIO_SHOULD_COPY TRUE)
else()
        set(TOBANTEAUDIO_SHOULD_COPY FALSE)
endif(APPLE)

juce_add_plugin(${PROJECT_NAME}
        PRODUCT_NAME "modEQ"                                    # The name of the final executable, which can differ from the target name
        VERSION ${CMAKE_PROJECT_VERSION}                        # Version
        FORMATS AU VST3 Standalone                              # The formats to build. Other valid formats are: AAX Unity VST AU AUv3
        IS_SYNTH FALSE                                          # Is this a synth or an effect?
        NEEDS_MIDI_INPUT FALSE                                  # Does the plugin need midi input?
        NEEDS_MIDI_OUTPUT FALSE                                 # Does the plugin need midi output?
        IS_MIDI_EFFECT FALSE                                    # Is this plugin a MIDI effect?
        EDITOR_WANTS_KEYBOARD_FOCUS FALSE                       # Does the editor need keyboard focus?
        COPY_PLUGIN_AFTER_BUILD ${TOBANTEAUDIO_SHOULD_COPY}     # Should the plugin be installed to a default location after building?
        PLUGIN_MANUFACTURER_CODE toAu                           # A four-character manufacturer id with at least one upper-case character
        PLUGIN_CODE tamq                                        # A unique four-character plugin id with at least one upper-case character
)

target_sources(${PROJECT_NAME} PRIVATE
        modEQ.hpp
        controller/analyser_controller.cpp
        controller/menu_bar_controller.cpp
        controller/modulation_source_controller.cpp
        controller/band_controller.cpp
        processor/equalizer_processor.cpp
        processor/modulation_source_processor.cpp
        parameters/text_value_converter.cpp
        look_and_feel/tobante_look_and_feel.cpp
        modEQ_processor.cpp
        render/svg.cpp
        modEQ_editor.cpp
        view/analyser_view.cpp
        view/band_view.cpp
        view/modulation_source_view.cpp
        view/settings_view.cpp
        view/modulation_connect_item_view.cpp
        view/social_buttons.cpp
        view/info_view.cpp
        view/menu_bar_view.cpp
        controller/analyser_controller.h
        controller/menu_bar_controller.h
        controller/modulation_source_controller.h
        controller/band_controller.h
        processor/base_processor.h
        processor/modulation_source_processor.h
        processor/equalizer_processor.h
        parameters/text_value_converter.h
        parameters/parameters.h
        analyser/modulation_source_analyser.h
        analyser/spectrum_analyser.h
        look_and_feel/tobante_look_and_feel.h
        modEQ_processor.h
        render/svg.h
        settings/constants.h
        view/modulation_connect_item_view.h
        view/social_buttons.h
        view/band_view.h
        view/settings_view.h
        view/menu_bar_view.h
        view/modulation_source_view.h
        view/info_view.h
        view/analyser_view.h
        modEQ_editor.h
        ${CMAKE_SOURCE_DIR}/Tests/test_main.cpp
        ${CMAKE_SOURCE_DIR}/Tests/test_main.h
        ${CMAKE_SOURCE_DIR}/Tests/test_text_converters.h
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/3rd_party)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/3rd_party/JUCE/modules)

target_compile_definitions(${PROJECT_NAME}
        PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
        JUCE_GLOBAL_MODULE_SETTINGS_INCLUDED=1
)
juce_generate_juce_header(${PROJECT_NAME})

juce_add_binary_data(modEQData NAMESPACE TobanteAudioData SOURCES
        ${CMAKE_SOURCE_DIR}/resource/material-icons/outline-high_quality-24px.svg
        ${CMAKE_SOURCE_DIR}/resource/material-icons/outline-info-24px.svg
        ${CMAKE_SOURCE_DIR}/resource/material-icons/outline-power_settings_new-24px.svg
        ${CMAKE_SOURCE_DIR}/resource/material-icons/outline-public-24px.svg
        ${CMAKE_SOURCE_DIR}/resource/material-icons/outline-redo-24px.svg
        ${CMAKE_SOURCE_DIR}/resource/material-icons/outline-settings-24px.svg
        ${CMAKE_SOURCE_DIR}/resource/material-icons/outline-settings_input_svideo-24px.svg
        ${CMAKE_SOURCE_DIR}/resource/material-icons/outline-undo-24px.svg
        ${CMAKE_SOURCE_DIR}/resource/social-media/github.svg
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        modEQData
        tobanteAudio::CompilerWarnings
        tobanteAudio::CompilerOptions
        tobanteAudio::CodeCoverage
        juce::juce_audio_utils
        juce::juce_dsp
        ff_meters
        )