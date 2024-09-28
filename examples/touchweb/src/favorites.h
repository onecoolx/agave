/* favorites.h - Agave application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#ifndef _FAVORITES_H_
#define _FAVORITES_H_

#include <vector>

class FavoritesImpl;

class Favorites
{
public:
    typedef struct {
        int id;
        std::string title;
        std::string url;
        std::string tags;
    } Item;

    Favorites();
    ~Favorites();

    void increaseCount(int id);
    void deleteFavortesByID(int id);
    std::vector<Item> getFavoritesItems(unsigned int offset, unsigned int nums);
    std::vector<Item> searchFavoritesItems(const std::string& utf8_key, unsigned int offset, unsigned int nums);
    unsigned int getFavoritesCount(void);
    void addFavorites(const std::string& utf8_title, const std::string& utf8_url,
                      const std::string& utf8_tags = std::string(), const std::string& utf8_note = std::string());
private:
    FavoritesImpl* m_impl;
};

#endif/*_FAVORITES_H_*/
