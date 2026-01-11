#pragma once

#include <string>
#include <vector>

namespace ProphecyTexts {

    // Helper: {} serves as the countdown placeholder
    
    inline std::vector<std::string> GRAVITY_FLIP = {
        "gravity flipping in {}...",
        "upside down in {}...",
        "floor becomes ceiling in {}...",
        "get ready to flip... {}",
        "gravity reverse in {}..."
    };
    
    inline std::vector<std::string> SPEED_CHANGE = {
        "speed change in {}...",
        "velocity shifting in {}...",
        "going fast/slow in {}...",
        "speed modification... {}"
    };
    
    inline std::vector<std::string> SIZE_CHANGE = {
        "size changing in {}...",
        "shrinking or growing in {}...",
        "morphing in {}...",
        "hitbox change... {}"
    };
    
    inline std::vector<std::string> LEVEL_COMPLETE = {
        "auto-win in {}...",
        "level completion in {}...",
        "victory imminent... {}",
        "ez dub in {}..."
    };
    
    inline std::vector<std::string> SPIKES_REMOVED = {
        "removing spikes in {}...",
        "hazard deletion in {}...",
        "safety mode in {}...",
        "spikes gone in {}..."
    };

    inline std::string DISABLED = "Prophecies Disabled";

    // ANIME MODE
    inline std::vector<std::string> ANIME_GRAVITY_FLIP = {
        "kyaa~! flipping in {}~!",
        "upside down time! {} left~",
        "whoosh! gravity change in {}~",
        "get ready senpai! {}~"
    };
    
    inline std::vector<std::string> ANIME_SPEED_CHANGE = {
        "speeding up/down! {}~",
        "zoom zoom? or slow mo? {}~",
        "velocity change in {} desu~",
        "pay attention! {}~"
    };
    
    inline std::vector<std::string> ANIME_SIZE_CHANGE = {
        "morphing time! {}~",
        "big or small? {} to see~",
        "transformation in {}~!",
        "poof! changing size in {}~"
    };
    
    inline std::vector<std::string> ANIME_LEVEL_COMPLETE = {
        "yay! winning in {}~!",
        "victory for you! {} left~",
        "you did it! (almost) {}~",
        "sugoi! clear in {}~"
    };
    
    inline std::vector<std::string> ANIME_SPIKES_REMOVED = {
        "spikes go bye-bye in {}~",
        "making it safe! {}~",
        "begone spikes! {}~",
        "protecting you in {}~"
    };

    inline std::string DISABLED_MESSAGE = "Prophecies Disabled";
    inline std::string ANIME_DISABLED_MESSAGE = "Prophecies Disabled (Anime Mode)";

    // ========== CHARACTER NAMES ==========
    inline std::string NORMAL_NAME = "BRODY";
    inline std::string ANIME_NAME = "BRODY-CHAN";
}
