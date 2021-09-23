#include "Common.h"

#include <list>
#include <unordered_map>
#include <mutex>

using namespace std;

class LruCache : public ICache {
  struct RankItem;
  struct StorageItem;

  using Rank = list<RankItem>;

  struct StorageItem {
    BookPtr book_ptr;
    Rank::iterator rank_it;
  };
  
  using Storage = unordered_map<string, StorageItem>;
  
  struct RankItem {
    size_t size;
    Storage::iterator storage_it;
  };

public:
  LruCache(
      shared_ptr<IBooksUnpacker> books_unpacker,
      const Settings& settings
  ): books_unpacker_(move(books_unpacker)), settings_(settings) {}

  BookPtr GetBook(const string& book_name) override {
    BookPtr book;
    lock_guard lg(m_);
    auto it = storage_.find(book_name);
    if(it == storage_.end()) {
    
      book = books_unpacker_->UnpackBook(book_name);
      size_t size = book->GetContent().size();

      bool too_big = false;
      while(total_size_ + size > settings_.max_memory) {
        if(rank_.empty()) {
          too_big = true;
          break;
        }
        const RankItem& item = rank_.back();
        total_size_ -= item.size;
        storage_.erase(item.storage_it);
        rank_.pop_back();
      }

      if(!too_big) {
        it = storage_.insert({book_name, {book, {}}}).first;
        rank_.push_front({size, it});
        it->second.rank_it = rank_.begin();
        total_size_ += size;
      }
    
    } else {
    
      book = it->second.book_ptr;
      size_t size = book->GetContent().size();
      
      rank_.erase(it->second.rank_it);
      rank_.push_front({size, it});
      it->second.rank_it = rank_.begin();
    
    }
    
    return book;
  }

private:
  mutex m_;
  shared_ptr<IBooksUnpacker> books_unpacker_;
  Settings settings_;
  Storage storage_;
  Rank rank_;
  size_t total_size_ = 0;
};


unique_ptr<ICache> MakeCache(
    shared_ptr<IBooksUnpacker> books_unpacker,
    const ICache::Settings& settings
) {
  return make_unique<LruCache>(move(books_unpacker), settings);
}
