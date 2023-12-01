#include "tiktok.h"
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
/**
 * Compares two pairs of hashtags and their hashtag, associated usage, and view counts
 * Priority is based on: greater # of uses then
 *                       greater # of views if the # of uses are equivalent then 
 *                       smaller string size if the # of uses and views are equivalent
 * 
 * @param  a      : First pair containing a hashtag, and its total use and total view count
 * @param  b      : Second pair containing a hashtag, and its total use and total view count
 * @return {bool} : True if b has greater priority than a. False otherwise
 */
bool compareHashtag::operator()(const std::pair<std::string, std::pair<unsigned long long, unsigned long long> > &a, 
                                const std::pair<std::string, std::pair<unsigned long long, unsigned long long> > &b) const
{
    // Parameter format:
    // {hashtag, {total use, total view}}
    const std::string &aHashtag = a.first;
    const std::string &bHashtag = b.first;
    const unsigned long long &aTotalUse = a.second.first;
    const unsigned long long &bTotalUse = b.second.first;
    const unsigned long long &aTotalView = a.second.second;
    const unsigned long long &bTotalView = b.second.second;

    if((aTotalUse < bTotalUse) ||
       (aTotalUse == bTotalUse && aTotalView < bTotalView) || 
       (aTotalUse == bTotalUse && aTotalView == bTotalView && aHashtag > bHashtag))
    {
        return true;
    }
    return false;
}
/**
 * Compares two pairs of soundID and their associated view counts
 * Priority is based on: greater # of views
 *                       smaller string size if the # of views are equivalent
 * 
 * @param  a      : First pair containing a soundID and total view count
 * @param  b      : Second pair containing a soundID and total view count
 * @return {bool} : True if b has greater priority than a. False otherwise
 */
bool compareSound::operator()(const std::pair<std::string, unsigned long long> &a, 
                              const std::pair<std::string, unsigned long long> &b) const
{
    const std::string &aSoundID = a.first;
    const std::string &bSoundID = b.first;
    const unsigned long long &aTotalView = a.second;
    const unsigned long long &bTotalView = b.second;

    if((aTotalView < bTotalView) ||
       (aTotalView == bTotalView && aSoundID > bSoundID))
    {
        return true;
    }
    return false;
}
/**
 * Parses a given text and extracts all hashtags from it
 * 
 * @param  text                        : Input string from which hashtags are to be extracted
 * @return {std::vector<std::string>}  : A vector of strings where each string is a
 *                                       hashtag extracted from the input text
 */
std::vector<std::string> parseHashtag(const std::string &text)
{
    std::vector<std::string> output;

    // define a regular expression to match hashtags
    std::regex hashtagRegex("#([\\w\\u0080-\\uFFFF]+)");

    // create an iterator for matching
    std::sregex_iterator hashtagIterator(text.begin(), text.end(), hashtagRegex);
    std::sregex_iterator endIterator;

    // iterate over the matches and extract the hashtags
    while (hashtagIterator != endIterator) 
    {
        std::smatch match = *hashtagIterator;
        std::string hashtag = match.str(1);  // extract the first capturing group

        output.push_back(hashtag);

        ++hashtagIterator;
    }
    return output;
}
/**
 * Reads Tiktok data formated in a JSON format and populates an unordered map based on the specified mode.
 * 
 * @param  input : input stream for reading the JSON file
 * @param  map   : Unordered map storing TikTok data based on either hashtags or sounds
 * @param  mode  : String indicating the mode of parsing ("hashtag" or "sound")
 */
void parseJSON(std::ifstream &input, 
               std::unordered_map<std::string, std::pair<std::priority_queue<TikTok>, unsigned long long> > &map, 
               const std::string &mode)
{
    std::string line;
    // Read each line of the input and creates a Tiktok object with each line
    while(std::getline(input, line)) 
    {
        // Reads JSON
        std::string ID, text, coverURL, webURL, soundName, soundAuthor, soundID;
        unsigned long long viewCount;

        size_t IDTagEnd = line.find("\"id\": ") + 6;
        size_t IDStart = line.find_first_not_of("\"", IDTagEnd);
        size_t IDEnd = line.find("\"", IDStart);
        ID = line.substr(IDStart, IDEnd - IDStart);

        size_t textTagEnd = line.find("\"text\": ") + 8;
        size_t textStart = line.find_first_not_of("\"", textTagEnd);
        size_t textEnd = line.find("\"", textStart);
        text = line.substr(textStart, textEnd - textStart);

        size_t coverURLTagEnd = line.find("\"coverUrl\": ") + 12;
        size_t coverURLStart = line.find_first_not_of("\"", coverURLTagEnd);
        size_t coverURLEnd = line.find("\"", coverURLStart);
        coverURL = line.substr(coverURLStart, coverURLEnd - coverURLStart);

        size_t webURLTagEnd = line.find("\"webVideoUrl\": ") + 15;
        size_t webURLStart = line.find_first_not_of("\"", webURLTagEnd);
        size_t webURLEnd = line.find("\"", webURLStart);
        webURL = line.substr(webURLStart, webURLEnd - webURLStart);

        size_t soundNameTagEnd = line.find("\"musicName\": ") + 13;
        size_t soundNameStart = line.find_first_not_of("\"", soundNameTagEnd);
        size_t soundNameEnd = line.find("\"", soundNameStart);
        soundName = line.substr(soundNameStart, soundNameEnd - soundNameStart);

        size_t soundAuthorTagEnd = line.find("\"musicAuthor\": ") + 15;
        size_t soundAuthorStart = line.find_first_not_of("\"", soundAuthorTagEnd);
        size_t soundAuthorEnd = line.find("\"", soundAuthorStart);
        soundAuthor = line.substr(soundAuthorStart, soundAuthorEnd - soundAuthorStart);

        size_t soundIDTagEnd = line.find("\"musicId\": ") + 11;
        size_t soundIDStart = line.find_first_not_of("\"", soundIDTagEnd);
        size_t soundIDEnd = line.find("\"", soundIDStart);
        soundID = line.substr(soundIDStart, soundIDEnd - soundIDStart);

        size_t viewCountTagEnd = line.find("\"playCount\": ") + 13;
        size_t viewCountStart = line.find_first_not_of("\"", viewCountTagEnd);
        size_t viewCountEnd = line.find("\"", viewCountStart);
        viewCount = std::stoi(line.substr(viewCountStart, viewCountEnd - viewCountStart));
       
        // TikTok object with the extracted attributes from JSON line
        TikTok video(ID, text, coverURL, webURL, soundName, soundAuthor, soundID, viewCount);

        if(mode == "hashtag")
        {
            // Extract all hashtags from the TikTok text
            const std::vector<std::string> &hashtags = parseHashtag(text);

            // Associate the TikTok object with each hashtag and update total view count
            for(const std::string &hashtag : hashtags)
            {
                map[hashtag].first.push(video);   
                map[hashtag].second += viewCount;
            }
        }
        else if(mode == "sound")
        {
            // Associate the TikTok object with the corresponding soundID and update total view count
            map[soundID].first.push(video);
            map[soundID].second += viewCount; 
        }      
    }
}

/**
 * Prints information about the top trending hashtags to an output stream
 * 
 * @param  out_str       : Output stream
 * @param  top10Hashtags : Vector containing the top 10 trending hashtags
 * @param  map           : Unordered map containing TikTok data associated with hashtags
 */
void printTopHashtags(std::ofstream &out_str, 
                      const std::vector<std::string> &top10Hashtags,
                      const std::unordered_map<std::string, std::pair<std::priority_queue<TikTok>, 
                                                                      unsigned long long> > &map)
{
    out_str << "trending hashtags:\n" << std::endl;
    // Iterate through top 10 hashtags
    for(int x = 0; x < top10Hashtags.size(); x++)
    {
        const std::string &hashtag = top10Hashtags[x];
        const std::pair<std::priority_queue<TikTok>, unsigned long long> &hashtagData = map.at(hashtag);
        std::priority_queue<TikTok> pq = hashtagData.first;
        out_str << "========================\n"
                << "#" << hashtag << "\n"
                << "used " << pq.size() << " times\n"
                << hashtagData.second << " views\n\n";
                // For each hashtag, print the video info of up to 3 Tiktoks
                for(int y = 0; y < 3; y++)
                {
                    if(pq.empty()) break;
                    const TikTok &tiktok = pq.top();
                    out_str << "cover url: " << tiktok.coverURL << "\n"
                            << "web video url: " << tiktok.webURL << "\n";
                    pq.pop();
                }
        out_str << "========================\n";
    }
}
/**
 * Prints information about the top trending sounds to an output stream
 * 
 * @param  out_str     : Output stream
 * @param  top10Sounds : Vector containing the top 10 trending soundID
 * @param  map         : Unordered map containing TikTok data associated with soundID
 */
void printTopSounds(std::ofstream &out_str, 
                    const std::vector<std::string> &top10Sounds,
                    const std::unordered_map<std::string, std::pair<std::priority_queue<TikTok>, 
                                                                    unsigned long long> > &map)
{
    out_str << "trending sounds:\n" << std::endl;

    // Iterate through top 10 soundID
    for(int x = 0; x < top10Sounds.size(); x++)
    {
        const std::string &soundID = top10Sounds[x];
        const std::pair<std::priority_queue<TikTok>, unsigned long long> &soundData = map.at(soundID);
        std::priority_queue<TikTok> pq = soundData.first;

        // Gets soundName and soundAuthor from first video
        const TikTok &topTikTok = pq.top();
        std::string soundName = topTikTok.soundName;
        std::string soundAuthor = topTikTok.soundAuthor;

        out_str << "========================\n"
                << soundName << "\n"
                << soundData.second << " views\n"
                << soundAuthor << "\n"
                << "music id: " << soundID << "\n\n";
                // For each sound, print the video info of up to 3 Tiktoks
                for(int y = 0; y < 3; y++)
                {
                    if(pq.empty()) break;
                    const TikTok &tiktok = pq.top();
                    out_str << "cover url: " << tiktok.coverURL<< "\n"
                            << "web video url: " << tiktok.webURL << "\n";
                    pq.pop();
                }
        out_str << "========================\n";
    }
}
/**
 * Compare TikTok objects based on their view counts
 * 
 * @param  a      : First TikTok object for comparison
 * @param  b      : Second TikTok object for comparison
 * @return {bool} : True if the view count of TikTok 'a' is less than the view count of TikTok 'b'.
 *                  False otherwise
 */
bool operator<(const TikTok &a, const TikTok &b)
{
    return a.viewCount < b.viewCount;
}
