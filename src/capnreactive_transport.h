#pragma once

#ifdef SUNSHINE_CAPNREACTIVE_ENABLED

#include <string>
#include <cstdint>
#include <cstddef>

namespace capnreactive_transport {

bool initialize_capnreactive_transport(const std::string& bind_addr);

bool start_capnreactive_video_streaming(uint32_t width, uint32_t height, uint32_t fps);
bool start_capnreactive_audio_streaming(uint32_t sample_rate, uint32_t channels);
bool initialize_capnreactive_input_handling();

bool send_capnreactive_video_frame(const void* frame_data, size_t frame_size, uint32_t timestamp);
bool send_capnreactive_audio_data(const void* audio_data, size_t data_size, uint32_t timestamp);

bool is_capnreactive_initialized();
void shutdown_capnreactive_transport();

}

#endif // SUNSHINE_CAPNREACTIVE_ENABLED
