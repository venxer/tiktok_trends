#include "tiktok.h"

#include <string>
#include <unordered_map>
#include <queue>
#include <iostream>

TikTok::TikTok()
{
    ID = "";
    text = "";
    coverURL = "";
    webURL = "";
    soundName = "";
    soundAuthor = "";
    soundID = "";
    viewCount = 0;
}

TikTok::TikTok(std::string ID,
               std::string text,
               std::string coverURL,
               std::string webURL,
               std::string soundName,
               std::string soundAuthor,
               std::string soundID,
               unsigned long long viewCount)
{
    this->ID = ID;
    this->text = text;
    this->coverURL = coverURL;
    this->webURL = webURL;
    this->soundName = soundName;
    this->soundAuthor = soundAuthor;
    this->soundID = soundID;
    this->viewCount = viewCount;
}

 bool compareHashtag::operator()(const std::pair<std::string, std::pair<unsigned long long, unsigned long long> > &a, 
                                 const std::pair<std::string, std::pair<unsigned long long, unsigned long long> > &b) const 
{
    /*
    first hashtag
    second.first total use
    second.second total view
    */
    std::string aHashtag = a.first;
    std::string bHashtag = b.first;
    unsigned long long aTotalUse = a.second.first;
    unsigned long long bTotalUse = b.second.first;
    unsigned long long aTotalView = a.second.second;
    unsigned long long bTotalView = b.second.second;

    if((aTotalUse < bTotalUse) ||
        (aTotalUse == bTotalUse && aTotalView < bTotalView) || 
        (aTotalUse == bTotalUse && aTotalView == bTotalView && aHashtag > bHashtag))
    {
        return true;
    }
    return false;
}
bool compareSound::operator() (const std::pair<std::string, unsigned long long> &a, 
                               const std::pair<std::string, unsigned long long> &b) const
{
    std::string aSoundID = a.first;
    std::string bSoundID = b.first;
    unsigned long long aTotalView = a.second;
    unsigned long long bTotalView = b.second;

    if((aTotalView < bTotalView) ||
        (aTotalView == bTotalView && aSoundID > bSoundID))
    {
        return true;
    }
    return false;
}
void printTopHashtags(std::ofstream &out_str, 
                      const std::vector<std::string> &top10Hashtags,
                      const std::unordered_map<std::string, std::pair<std::priority_queue<TikTok>, unsigned long long> > &map)
{
    out_str << "trending hashtags:\n" << std::endl;
    for(int x = 0; x < top10Hashtags.size(); x++)
    {
        std::string hashtag = top10Hashtags[x];
        std::pair<std::priority_queue<TikTok>, unsigned long long> hashtagData = map.find(hashtag)->second;
        std::priority_queue<TikTok> pq = hashtagData.first;
        out_str << "========================\n"
                << "#" << hashtag << "\n"
                << "used " << pq.size() << " times\n"
                << hashtagData.second << " views\n\n";
                for(int y = 0; y < 3 && !pq.empty(); y++)
                {
                    TikTok tiktok = pq.top();
                    out_str << "cover url: " << tiktok.coverURL << "\n"
                            << "web video url: " << tiktok.webURL << "\n";
                    pq.pop();
                }
        out_str << "========================\n";
    }
}
void printTopSounds(std::ofstream &out_str, 
                    const std::vector<std::string> &top10Sounds,
                    const std::unordered_map<std::string, std::pair<std::priority_queue<TikTok>, unsigned long long> > &map)
                    
{
    out_str << "trending sounds:\n" << std::endl;
    for(int x = 0; x < top10Sounds.size(); x++)
        {
            std::string soundID = top10Sounds[x];
            std::pair<std::priority_queue<TikTok>, unsigned long long> soundData = map.find(soundID)->second;
            std::priority_queue<TikTok> pq = soundData.first;
            TikTok topTikTok = pq.top();
            std::string soundName = topTikTok.soundName;
            std::string soundAuthor = topTikTok.soundAuthor;

            out_str << "========================\n"
                    << soundName << "\n"
                    << soundData.second << " views\n"
                    << soundAuthor << "\n"
                    << "music id: " << soundID << "\n\n";
                    for(int y = 0; y < 3 && !pq.empty(); y++)
                    {
                        TikTok tiktok = pq.top();
                        out_str << "cover url: " << tiktok.coverURL<< "\n"
                                << "web video url: " << tiktok.webURL << "\n";
                        pq.pop();
                    }
            out_str << "========================\n";
        }
}
bool operator<(const TikTok &a, const TikTok &b)
{
    return a.viewCount < b.viewCount;
}
