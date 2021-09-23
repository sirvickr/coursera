#pragma once

namespace RAII {

  template<class Provider>
  class Booking {
  private:
    Provider* provider_;

  public:
    Booking(Provider* provider, int)
      : provider_(provider) 
    {}

    Booking(const Booking&) = delete;

    Booking(Booking&& other)
      : provider_(other.provider_) 
    {
      other.provider_ = nullptr;
    }

    Booking& operator=(const Booking&) = delete;

    Booking& operator=(Booking&& other) {
      provider_ = other.provider_;
      other.provider_ = nullptr;
      return *this;
    }

    ~Booking() {
      if(provider_) {
        provider_->CancelOrComplete(*this);
      }
    }
  };

}
