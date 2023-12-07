#include "tiktok.h"

#include <iostream>

/**
 * Check if file can successfully be opened
 * 
 * @param fileName name of file to be opened
 *      
 * @return the opened file stream
 */
template<typename T>
T validateFile(const std::string &fileName);

int main(int argc, char const *argv[])
{
    try
    {
        // Check argument count
        if(argc != 4)
        {
            throw std::invalid_argument("Invalid Argument Count");
        }

        std::ifstream input = validateFile<std::ifstream>(argv[1]);
        std::ofstream output = validateFile<std::ofstream>(argv[2]);
        std::string mode = argv[3];

        // Format of map:
        // {hashtag, {Tiktok sorted by views, total views}}
        // {soundID, {Tiktok sorted by views, total views}}
        std::unordered_map<std::string, std::pair<std::priority_queue<TikTok>, unsigned long long> > map;

        // Fills up map based on input data and mode
        parseJSON(input, map, mode);

        if(mode == "hashtag")
        {
            std::priority_queue<std::pair<std::string, std::pair<unsigned long long, unsigned long long> >, 
                                std::vector<std::pair<std::string, std::pair<unsigned long long, unsigned long long> > >,
                                compareHashtag> topHashtags;

            // Get each hashtag and the data correlated with each hashtag to organize a priority queue with the top hashtags
            for(std::pair<std::string, std::pair<std::priority_queue<TikTok>, unsigned long long>> hashtagEntry : map) 
            {
                // Format of hashtagData:
                // {hashtag, {total use, total views}}
                std::pair<unsigned long long, unsigned long long> useAndView(hashtagEntry.second.first.size(), hashtagEntry.second.second);
                std::pair<std::string, 
                          std::pair<unsigned long long, unsigned long long> > hashtagData (hashtagEntry.first,useAndView);
                topHashtags.push(hashtagData);
            }

            // Pushes top 10 hashtags from priority queue into a vector;
            std::vector<std::string> top10Hashtags;
            for(int x = 0; x < 10; x++)
            {
                // Exit loop if sub 10 total unique hashtags
                if (topHashtags.empty()) break;

                top10Hashtags.push_back(topHashtags.top().first);
                topHashtags.pop();
            }

            // Prints top 10 hashtags with top 3 videos in required format
            printTopHashtags(output, top10Hashtags, map);
        }
        else if(mode == "sound")
        {
            std::priority_queue<std::pair<std::string, unsigned long long>,
                                std::vector<std::pair<std::string, unsigned long long> >,
                                compareSound> topSounds;

            // Get each soundID and the data correlated with each soundID to organize a priority queue with the top sounds
            for(std::pair<std::string, std::pair<std::priority_queue<TikTok>, unsigned long long>> soundEntry : map)
            {
                // Format of soundData:
                // {soundID, total views}
                std::pair<std::string, unsigned long long> soundData(soundEntry.first, soundEntry.second.second);
                topSounds.push(soundData);
            }

            // Pushes top 10 sounds from priority queue into a vector
            std::vector<std::string> top10Sounds;
            for(int x = 0; x < 10; x++)
            {
                // Exit loop if sub 10 total unique soundID
                if (topSounds.empty()) break;

                top10Sounds.push_back(topSounds.top().first);
                topSounds.pop();
            }

            // Prints top 10 sounds with top 3 videos in required format
            printTopSounds(output, top10Sounds, map);
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    
    return 0;
}

template<typename T>
T validateFile(const std::string &fileName)
{
    T stream(fileName);
    if(!stream.good()) std::cerr << "File Error" << std::endl;
    return stream;
}
