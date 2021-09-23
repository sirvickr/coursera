#pragma once

#include <utility>

namespace RAII {

  template<class Provider>
  class Booking {
  private:
    using BookingId = typename Provider::BookingId;
    
    Provider* provider_;
    BookingId id_;

  public:
    Booking(Provider* provider, const BookingId& id)
      : provider_(provider)
      , id_(id)
    {}

    Booking(const Booking&) = delete;

    Booking(Booking&& other)
      : provider_(other.provider_) 
      , id_(other.id_)
    {
      other.provider_ = nullptr;
    }

    Booking& operator=(const Booking&) = delete;

    Booking& operator=(Booking&& other) {
      std::swap(provider_, other.provider_);
      std::swap(id_, other.id_);
      return *this;
    }

    // Эта функция не требуется в тестах, но в реальной программе она может быть нужна
    BookingId GetId() const {
      return id_;
    }

    ~Booking() {
      if(provider_) {
        provider_->CancelOrComplete(*this);
      }
    }
  };

}
