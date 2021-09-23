#include "Common.h"

#include <list>
#include <unordered_map>
#include <mutex>
#include <cassert>
#include <algorithm>

using namespace std;

class LruCache : public ICache {
public:
  LruCache(
      shared_ptr<IBooksUnpacker> books_unpacker,
      const Settings& settings
  )
    : books_unpacker_(move(books_unpacker))
    , settings_(settings)
  {}

  BookPtr GetBook(const string& book_name) override {
    lock_guard guard(mutex_);

    BookPtr book;

    auto it = books_.find(book_name);

    if(it == books_.end()) {
    
      book = books_unpacker_->UnpackBook(book_name);

      size_t size = book->GetContent().size();
      bool too_big = false;
      while(cache_size_ + size > settings_.max_memory) {
        if(ranks_.empty()) {
          too_big = true;
          break;
        }
        RemoveLruEntry();
      }

      if(!too_big) {
        it = books_.insert({book_name, {book, {}}}).first;
        cache_size_ += size;
        AddLruEntry(size, it);
      }
    
    } else {
    
      book = it->second.book_ptr;
      
      ranks_.erase(it->second.rank_it);
      AddLruEntry(book->GetContent().size(), it);
    
    }
    
    return book;
  }

private:
  struct RankItem;
  struct Entry;

  using Rank = list<RankItem>;

  struct Entry {
    BookPtr book_ptr;
    Rank::iterator rank_it;
  };
  
  using Entries = unordered_map<string, Entry>;
  
  struct RankItem {
    size_t size;
    Entries::iterator books_it;
  };

private:
  void RemoveLruEntry() {
    const RankItem& item = ranks_.back();
    cache_size_ -= item.size;
    books_.erase(item.books_it);
    ranks_.pop_back();
  }

  void AddLruEntry(size_t size, Entries::iterator it) {
      ranks_.push_front({size, it});
      it->second.rank_it = ranks_.begin();
  }

private:
  shared_ptr<IBooksUnpacker> books_unpacker_;
  const Settings settings_;

  mutex mutex_;
  Entries books_;
  Rank ranks_;
  size_t cache_size_ = 0;
};


unique_ptr<ICache> MakeCache(
    shared_ptr<IBooksUnpacker> books_unpacker,
    const ICache::Settings& settings
) {
  return make_unique<LruCache>(move(books_unpacker), settings);
}
