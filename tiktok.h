#ifndef TIKTOK_H
#define TIKTOK_H

#include <string>
#include <unordered_map>
#include <queue>
#include <fstream>
#include <regex>

class TikTok
{
    private:
        std::string ID;
        std::string text;
        std::string coverURL;
        std::string webURL;
        std::string soundName;
        std::string soundAuthor;
        std::string soundID;
        unsigned long long viewCount;
        
    public:
        TikTok();
        TikTok(std::string ID,
               std::string text,
               std::string coverURL,
               std::string webURL,
               std::string soundName,
               std::string soundAuthor,
               std::string soundID,
               unsigned long long viewCount);

        std::string getID()                  {return ID;          };
        std::string getText()                {return text;        };
        std::string getCoverURL()            {return coverURL;    };
        std::string getWebURL()              {return webURL;      };
        std::string getSoundName()           {return soundName;   };
        std::string getSoundAuthor()         {return soundAuthor; };
        std::string getSoundID()             {return soundID;     };
        unsigned long long getViewCount()    {return viewCount;   };

        friend void printTopHashtags(std::ofstream &out_str, 
                                     const std::vector<std::string> &top10Hashtags,
                                     const std::unordered_map<std::string, std::pair<std::priority_queue<TikTok>, 
                                                                                     unsigned long long> > &map);
        friend void printTopSounds(std::ofstream &out_str, 
                                   const std::vector<std::string> &top10Sounds,
                                   const std::unordered_map<std::string, std::pair<std::priority_queue<TikTok>, 
                                                                                   unsigned long long> > &map);
        friend bool operator<(const TikTok &a, const TikTok &b);
};

class compareHashtag
{
    public:
        bool operator()(const std::pair<std::string, std::pair<unsigned long long, unsigned long long> > &a, 
                        const std::pair<std::string, std::pair<unsigned long long, unsigned long long> > &b) const;
};
class compareSound
{
    public:
        bool operator() (const std::pair<std::string, unsigned long long> &a, 
                         const std::pair<std::string, unsigned long long> &b) const;
};

std::vector<std::string> parseHashtag(const std::string &text);
void parseJSON(std::ifstream &input, 
               std::unordered_map<std::string, std::pair<std::priority_queue<TikTok>, unsigned long long> > &map, 
               const std::string &mode);
void printTopHashtags(std::ofstream &out_str, 
                      const std::vector<std::string> &top10Hashtags,
                      const std::unordered_map<std::string, std::pair<std::priority_queue<TikTok>, 
                                                                      unsigned long long> > &map);
void printTopSounds(std::ofstream &out_str, 
                    const std::vector<std::string> &top10Sounds,
                    const std::unordered_map<std::string, std::pair<std::priority_queue<TikTok>, 
                                                                    unsigned long long> > &map);
bool operator<(const TikTok &a, const TikTok &b);

#endif

