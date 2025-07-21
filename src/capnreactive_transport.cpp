#ifdef SUNSHINE_CAPNREACTIVE_ENABLED

#include "capnreactive_transport.h"
#include "sunshine_capnreactive.h"
#include "logging.h"
#include "network.h"

namespace capnreactive_transport {

class SunshineCapnReactiveManager {
  private:
    CSunshineClient *client_;
    bool initialized_;

  public:
    SunshineCapnReactiveManager() : client_(nullptr), initialized_(false) {}

    ~SunshineCapnReactiveManager() {
      if (client_) {
        sunshine_capnreactive_destroy(client_);
      }
    }

    bool
    initialize(const std::string &bind_addr) {
      client_ = sunshine_capnreactive_create();
      if (!client_) {
        BOOST_LOG(error) << "Failed to create CapnReactive client";
        return false;
      }

      int result = sunshine_capnreactive_init_transport(client_, bind_addr.c_str());
      if (result != SUNSHINE_SUCCESS) {
        BOOST_LOG(error) << "Failed to initialize CapnReactive transport: " << result;
        return false;
      }

      initialized_ = true;
      BOOST_LOG(info) << "CapnReactive transport initialized successfully";
      return true;
    }

    bool
    start_video_streaming(uint32_t width, uint32_t height, uint32_t fps) {
      if (!initialized_) return false;

      int result = sunshine_capnreactive_start_video(client_, width, height, fps);
      if (result == SUNSHINE_SUCCESS) {
        BOOST_LOG(info) << "CapnReactive video streaming started: " << width << "x" << height << "@" << fps;
        return true;
      }
      return false;
    }

    bool
    start_audio_streaming(uint32_t sample_rate, uint32_t channels) {
      if (!initialized_) return false;

      int result = sunshine_capnreactive_start_audio(client_, sample_rate, channels);
      if (result == SUNSHINE_SUCCESS) {
        BOOST_LOG(info) << "CapnReactive audio streaming started: " << sample_rate << "Hz, " << channels << " channels";
        return true;
      }
      return false;
    }

    bool
    initialize_input_handling() {
      if (!initialized_) return false;

      int result = sunshine_capnreactive_init_input(client_);
      if (result == SUNSHINE_SUCCESS) {
        BOOST_LOG(info) << "CapnReactive input handling initialized";
        return true;
      }
      return false;
    }

    bool
    send_video_frame(const void *frame_data, size_t frame_size, uint32_t timestamp) {
      if (!initialized_) return false;

      int result = sunshine_capnreactive_send_video_frame(
        client_, frame_data, frame_size, timestamp);
      return result == SUNSHINE_SUCCESS;
    }

    bool
    send_audio_data(const void *audio_data, size_t data_size, uint32_t timestamp) {
      if (!initialized_) return false;

      int result = sunshine_capnreactive_send_audio_data(
        client_, audio_data, data_size, timestamp);
      return result == SUNSHINE_SUCCESS;
    }

    bool
    send_mouse_input(int x, int y, uint8_t buttons) {
      if (!initialized_) return false;

      int result = sunshine_capnreactive_send_mouse_input(client_, x, y, buttons);
      return result == SUNSHINE_SUCCESS;
    }

    bool
    send_keyboard_input(uint32_t key_code, bool pressed) {
      if (!initialized_) return false;

      int result = sunshine_capnreactive_send_keyboard_input(client_, key_code, pressed ? 1 : 0);
      return result == SUNSHINE_SUCCESS;
    }

    void
    get_video_stats(uint64_t &frames_sent, uint32_t &width, uint32_t &height, uint32_t &fps) {
      if (!initialized_) return;

      unsigned int frames, w, h, f;
      int result = sunshine_capnreactive_get_video_stats(client_, &frames, &w, &h, &f);
      if (result == SUNSHINE_SUCCESS) {
        frames_sent = frames;
        width = w;
        height = h;
        fps = f;
      }
    }

    bool
    is_initialized() const {
      return initialized_;
    }
};

static std::unique_ptr<SunshineCapnReactiveManager> g_capnreactive_manager;

bool
initialize_capnreactive_transport(const std::string &bind_addr) {
  g_capnreactive_manager = std::make_unique<SunshineCapnReactiveManager>();
  return g_capnreactive_manager->initialize(bind_addr);
}

bool
start_capnreactive_video_streaming(uint32_t width, uint32_t height, uint32_t fps) {
  if (!g_capnreactive_manager) return false;
  return g_capnreactive_manager->start_video_streaming(width, height, fps);
}

bool
start_capnreactive_audio_streaming(uint32_t sample_rate, uint32_t channels) {
  if (!g_capnreactive_manager) return false;
  return g_capnreactive_manager->start_audio_streaming(sample_rate, channels);
}

bool
initialize_capnreactive_input_handling() {
  if (!g_capnreactive_manager) return false;
  return g_capnreactive_manager->initialize_input_handling();
}

bool
send_capnreactive_video_frame(const void *frame_data, size_t frame_size, uint32_t timestamp) {
  if (!g_capnreactive_manager) return false;
  return g_capnreactive_manager->send_video_frame(frame_data, frame_size, timestamp);
}

bool
send_capnreactive_audio_data(const void *audio_data, size_t data_size, uint32_t timestamp) {
  if (!g_capnreactive_manager) return false;
  return g_capnreactive_manager->send_audio_data(audio_data, data_size, timestamp);
}

bool
is_capnreactive_initialized() {
  return g_capnreactive_manager && g_capnreactive_manager->is_initialized();
}

void
shutdown_capnreactive_transport() {
  g_capnreactive_manager.reset();
  BOOST_LOG(info) << "CapnReactive transport shutdown complete";
}

}

#endif // SUNSHINE_CAPNREACTIVE_ENABLED
