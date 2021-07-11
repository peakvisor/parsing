namespace prev_generated_mappings {

static constexpr size_t kRepetitions = 10;

using CGFloat = double;

struct DVGGeoTrail {
    // temporary; a subset of actual tags mapped for routing/rendering simplicity
    //      for the sake of renderer convenience has to be contiguous
    enum DVGTrailType : uint32_t { // BEWARE: implicitly assuming 0~(COUNT - 1) layout
        kDVGTrailTypeRiver,

        kDVGTrailTypeShortcut, // ...
        kDVGTrailTypeSteps, // steps
        kDVGTrailTypePath, // path, cycleway
        kDVGTrailTypePathGrade2, // && 1 < sac_scale <= 3
        kDVGTrailTypePathGrade3, // && 3 < sac_scale
        kDVGTrailTypeTrack, // track, pedestrian [and more]
        kDVGTrailTypeTrackGrade2, // && 1 < sac_scale [in practice 2-3]
        kDVGTrailTypeRoad, // service, residential, tertiary, unclassified
        kDVGTrailTypeWideRoad, // secondary
        kDVGTrailTypeMotorwayLink, // any "motorway" + _link
        kDVGTrailTypeMotorway, // primary, trunk, motorway

        // experimenting: joining the types + actually using per-vertex color would save some draw calls. but at what price?
        kDVGTrailTypePisteNovice,
        kDVGTrailTypePisteEasy,
        kDVGTrailTypePisteIntermediate,
        kDVGTrailTypePisteAdvanced,
        kDVGTrailTypePisteExpert,
        kDVGTrailTypePisteFreeride,
        kDVGTrailTypePisteExtreme, // slopes.waymarkedtrails.org doesn't render them; http://opensnowmap.org/?zoom=12&lon=8.6826&lat=47.1165& – has extensive styling

        // aerialways
        //      – closed/two-ways
        //      – open/one-way
        //      – railway
        // ~railway
        kDVGTrailTypeFunicular,
        kDVGTrailTypeNarrowGauge,

        // twoway cable cars
        kDVGTrailTypeCableCar,
        kDVGTrailTypeGondola,

        // oneway cable cars
        kDVGTrailTypeChairLift,
        kDVGTrailTypeDragLift,
        kDVGTrailTypeMagicCarpet,

        DVG_TRAIL_TYPES_COUNT,

        kDVGTrailTypeUnknown = DVG_TRAIL_TYPES_COUNT, // not yet treated types
        DVG_TRAIL_TYPE_DEFAULT = kDVGTrailTypeUnknown,
    };
};


struct DVGGeoDatabaseEntry {
    enum class PeakCategory : uint16_t {
        unknown = 0,
        
        ultra, top10, top50,
        k14er, k13er,
        volcano, glacier,
        munro, furth,
        tgo40, ja100, tw100, ahp46,
        
        COUNT
    };
    
    enum class Tag : uint64_t {
        unknown = 0,
        
        peakCategory,
        funicularType,
        area, // for glaciers, lakes, etc. in m^2
        population, // for places
        height, // for viewpoints
        
        relatedID, // for now used by peakCategories
        
        buildingType, // hotel, church, post-office, stupa, etc.
        amenityType,
        
        // compatibility?
        hutType,
        amenity,
        capacity,
        phone,
        url,
        // likely temporary:
        country,
        // TODO: for lakes,etc. relatedPolygon(s)?
        
        drinking, natural, shelterType, viewpointType,
        
        bookmarkMarkerShape,
        bookmarkMarkerColor,
        bookmarkName,
        bookmarkNote,
        checkInDate, // value: 64-bit uint seconds from from sorted ascending -- thus firstValueFor is the latest visit
        revision, // special-case tag: must be the first in the list to matter
        // TODO: isBagged/isVisited, wishList, etc.
        
        // TODO: manMadeHeight, placeType, prominenceBias, etc.
        
        COUNT
    };
    enum class AmenityType : uint64_t {
        unknown = 0, caveEntrance, picnicTable, shelter, water,
        COUNT
    };
    enum class HutType : uint64_t {
        unknown = 0, alpineHut, shelter, hotel, guestHouse, wildernessHut,
        COUNT
    };
    
    enum class Type : uint16_t {
        kUnknown = 0,
        
        kPeak,
        kCastle,
        kMountainHut,
        
        kViewpoint,
        kParking,
        
        kPiste, // TODO: not used and not going to?
        kFunicular,
        
        kPlace, // i.e. village/town/city
        kGlacier,
        kLake,
        kPass,
        
        kHotel, // TODO: temporary here?
        kAmenity, // TODO: not fully implemented // shall factor into -> picnic_table/cave_entrance/shelter/water
        kWaterfall, // TODO: not fully implemented
        kWaymark, // TODO: not fully implemented
        
        kCustomPoint,
        kUserLocation,
        kSun, kMoon, // TODO: debug
        
        COUNT
    };
};

using PeakCategory = DVGGeoDatabaseEntry::PeakCategory;
struct PeakCategoryMapping {
    using E = PeakCategory;
    static constexpr DVGKeyPair<E> values[]{
        {E::ultra, "Ultra"},
        {E::top10, "Top10"},
        {E::top50, "Top50"},
        {E::k14er, "14er"},
        {E::k13er, "13er"},
        {E::volcano, "Volcano"},
        {E::glacier, "Glacier"},
        {E::munro, "Munro"},
        {E::furth, "Furth"},
        {E::tgo40, "TGO40"},
        {E::ja100, "JA100"},
        {E::tw100, "TW100"},
        {E::ahp46, "AHP46"},
    };
    
    static inline __attribute__((always_inline)) E decode(const std::string_view &string) {
        if (string.size() < 4) { return E{}; }
        switch (string[1]) {
            case '3':
                return string == "13er" ? E::k13er : E{};
            case '4':
                return string == "14er" ? E::k14er : E{};
            case 'A':
                return string == "JA100" ? E::ja100 : E{};
            case 'G':
                return string == "TGO40" ? E::tgo40 : E{};
            case 'H':
                return string == "AHP46" ? E::ahp46 : E{};
            case 'W':
                return string == "TW100" ? E::tw100 : E{};
            case 'l':
                switch (string[3]) {
                    case 'c':
                        return string == "Glacier" ? E::glacier : E{};
                    default:
                        return string == "Ultra" ? E::ultra : E{};
                }
            case 'o':
                switch (string[3]) {
                    case '1':
                        return string == "Top10" ? E::top10 : E{};
                    case '5':
                        return string == "Top50" ? E::top50 : E{};
                    default:
                        return string == "Volcano" ? E::volcano : E{};
                }
            default:
                switch (string[3]) {
                    case 'r':
                        return string == "Munro" ? E::munro : E{};
                    default:
                        return string == "Furth" ? E::furth : E{};
                }
        }
        return E{};
    }
};

struct Legacy_EntryTypeMapping {
    using E = DVGGeoDatabaseEntry::Type;
    static constexpr DVGKeyPair<E> values[]{
        {E::kMountainHut, "alpine_hut"},
        {E::kCastle, "castle"},
        
        {E::kViewpoint, "viewpoint"},
        {E::kParking, "parking"}, // used only during resources preparation
        
        {E::kFunicular, "funicular_station"},
        
        {E::kViewpoint, "hotel"},
    };
    
    static inline __attribute__((always_inline)) E decode(const std::string_view &string) {
        if (string.size() < 5) { return E{}; }
        switch (string.size()) {
            case 5:
                return !std::memcmp(string.begin(), "hotel", 5) ? E::kViewpoint : E{};
            case 6:
                return !std::memcmp(string.begin(), "castle", 6) ? E::kCastle : E{};
            case 7:
                return !std::memcmp(string.begin(), "parking", 7) ? E::kParking : E{};
            case 9:
                return !std::memcmp(string.begin(), "viewpoint", 9) ? E::kViewpoint : E{};
            case 10:
                return !std::memcmp(string.begin(), "alpine_hut", 10) ? E::kMountainHut : E{};
            default:
                return string == "funicular_station" ? E::kFunicular : E{};
        }
        return E{};
    }
};

struct EntryTypeMapping {
    using E = DVGGeoDatabaseEntry::Type;
    static constexpr DVGKeyPair<E> values[]{
        {E::kMountainHut, "alpineHut"},
        {E::kAmenity, "amenity"}, //!
        {E::kCastle, "castle"},
        {E::kViewpoint, "viewpoint"},
        {E::kParking, "parking"},
        {E::kFunicular, "funicularStation"},
        {E::kWaterfall, "waterfall"},

        {E::kPlace, "place"},
        {E::kGlacier, "glacier"},
        {E::kLake, "lake"},
        {E::kPass, "pass"},

        {E::kPeak, "peak"}, // khm. used by d__bookmarkedEntriesCache only
    };

    static inline __attribute__((always_inline)) E decode(const std::string_view &string) {
        if (string.size() < 4) { return E{}; }
        switch (string.size()) {
            case 4:
                switch (string[2]) {
                    case 'a':
                        return !std::memcmp(string.begin(), "peak", 4) ? E::kPeak : E{};
                    case 'k':
                        return !std::memcmp(string.begin(), "lake", 4) ? E::kLake : E{};
                    default:
                        return !std::memcmp(string.begin(), "pass", 4) ? E::kPass : E{};
                }
            case 5:
                return !std::memcmp(string.begin(), "place", 5) ? E::kPlace : E{};
            case 6:
                return !std::memcmp(string.begin(), "castle", 6) ? E::kCastle : E{};
            case 7:
                switch (string[2]) {
                    case 'a':
                        return !std::memcmp(string.begin(), "glacier", 7) ? E::kGlacier : E{};
                    case 'e':
                        return !std::memcmp(string.begin(), "amenity", 7) ? E::kAmenity : E{};
                    default:
                        return !std::memcmp(string.begin(), "parking", 7) ? E::kParking : E{};
                }
            case 9:
                switch (string[2]) {
                    case 'e':
                        return !std::memcmp(string.begin(), "viewpoint", 9) ? E::kViewpoint : E{};
                    case 'p':
                        return !std::memcmp(string.begin(), "alpineHut", 9) ? E::kMountainHut : E{};
                    default:
                        return !std::memcmp(string.begin(), "waterfall", 9) ? E::kWaterfall : E{};
                }
            default:
                return string == "funicularStation" ? E::kFunicular : E{};
        }
        return E{};
    }
};

using FunicularType = DVGGeoTrail::DVGTrailType;
struct FunicularTypeMapping {
    using E = FunicularType;
    static constexpr DVGKeyPair<E> values[]{
        {E::kDVGTrailTypeFunicular, "funicular"},
        {E::kDVGTrailTypeNarrowGauge, "narrow_gauge"},
        {E::kDVGTrailTypeCableCar, "cable_car"},
        {E::kDVGTrailTypeGondola, "gondola"},
        {E::kDVGTrailTypeChairLift, "chair_lift"},
        {E::kDVGTrailTypeDragLift, "drag_lift"},
        {E::kDVGTrailTypeDragLift, "t-bar"}, // TODO: beware types coalescing
        {E::kDVGTrailTypeDragLift, "j-bar"},
        {E::kDVGTrailTypeDragLift, "platter"},
        {E::kDVGTrailTypeMagicCarpet, "magic_carpet"},
        {E::kDVGTrailTypeNarrowGauge, "rail"}, // TODO: beware types coalescing; at least icon/type-name shall differ
    };
    
    static inline __attribute__((always_inline)) E decode(const std::string_view &string) {
        if (string.size() < 4) { return E{}; }
        switch (string.size()) {
            case 4:
                return !std::memcmp(string.begin(), "rail", 4) ? E::kDVGTrailTypeNarrowGauge : E{};
            case 5:
                switch (string[0]) {
                    case 'j':
                        return !std::memcmp(string.begin(), "j-bar", 5) ? E::kDVGTrailTypeDragLift : E{};
                    default:
                        return !std::memcmp(string.begin(), "t-bar", 5) ? E::kDVGTrailTypeDragLift : E{};
                }
            case 7:
                switch (string[0]) {
                    case 'g':
                        return !std::memcmp(string.begin(), "gondola", 7) ? E::kDVGTrailTypeGondola : E{};
                    default:
                        return !std::memcmp(string.begin(), "platter", 7) ? E::kDVGTrailTypeDragLift : E{};
                }
            case 9:
                switch (string[0]) {
                    case 'c':
                        return !std::memcmp(string.begin(), "cable_car", 9) ? E::kDVGTrailTypeCableCar : E{};
                    case 'd':
                        return !std::memcmp(string.begin(), "drag_lift", 9) ? E::kDVGTrailTypeDragLift : E{};
                    default:
                        return !std::memcmp(string.begin(), "funicular", 9) ? E::kDVGTrailTypeFunicular : E{};
                }
            case 10:
                return !std::memcmp(string.begin(), "chair_lift", 10) ? E::kDVGTrailTypeChairLift : E{};
            default:
                switch (string[0]) {
                    case 'm':
                        return string == "magic_carpet" ? E::kDVGTrailTypeMagicCarpet : E{};
                    default:
                        return string == "narrow_gauge" ? E::kDVGTrailTypeNarrowGauge : E{};
                }
        }
        return E{};
    }
};

struct TagsMapping {
    using E = DVGGeoDatabaseEntry::Tag;
    static constexpr DVGKeyPair<E> values[]{
        {E::peakCategory, "category"},
        {E::funicularType, "funicularType"},
        
        {E::area, "area"}, // for glaciers, lakes, etc. in m^2
        {E::population, "population"}, // for places
        {E::relatedID, "relatedID"}, // for now used by peakCategories
        {E::height, "height"},
        
//        E::amenity, // hotel, church, post-office, stupa, etc.
        {E::amenityType, "amenityType"}, // at the moment: cave_entrance, picnic_table, shelter, water
        //  shelter -> shelterType; water -> drinking:yes/no | natural:spring;
        
        // for Type::alpineHut:
        {E::hutType, "hutType"},
        {E::amenity, "amenity"}, // restaurant, etc.
        {E::capacity, "capacity"},
        {E::phone, "phone"},
        {E::url, "url"},
        
        // used by aenityType=drinking_water:
        {E::drinking, "drinking"},
        {E::natural, "natural"},
        
        {E::shelterType, "shelterType"}, // for amenityType:shelter
        {E::viewpointType, "viewpointType"}, // for Type::viewpoint
    };
    
    static inline __attribute__((always_inline)) E decode(const std::string_view &string) {
        if (string.size() < 3) { return E{}; }
        switch (string.size()) {
            case 3:
                return !std::memcmp(string.begin(), "url", 3) ? E::url : E{};
            case 4:
                return !std::memcmp(string.begin(), "area", 4) ? E::area : E{};
            case 5:
                return !std::memcmp(string.begin(), "phone", 5) ? E::phone : E{};
            case 6:
                return !std::memcmp(string.begin(), "height", 6) ? E::height : E{};
            case 7:
                switch (string[0]) {
                    case 'a':
                        return !std::memcmp(string.begin(), "amenity", 7) ? E::amenity : E{};
                    case 'h':
                        return !std::memcmp(string.begin(), "hutType", 7) ? E::hutType : E{};
                    default:
                        return !std::memcmp(string.begin(), "natural", 7) ? E::natural : E{};
                }
            case 8:
                switch (string[0]) {
                    case 'c':
                        switch (string[2]) {
                            case 'p':
                                return !std::memcmp(string.begin(), "capacity", 8) ? E::capacity : E{};
                            default:
                                return !std::memcmp(string.begin(), "category", 8) ? E::peakCategory : E{};
                        }
                    default:
                        return !std::memcmp(string.begin(), "drinking", 8) ? E::drinking : E{};
                }
            case 9:
                return !std::memcmp(string.begin(), "relatedID", 9) ? E::relatedID : E{};
            case 10:
                return !std::memcmp(string.begin(), "population", 10) ? E::population : E{};
            case 11:
                switch (string[0]) {
                    case 'a':
                        return !std::memcmp(string.begin(), "amenityType", 11) ? E::amenityType : E{};
                    default:
                        return !std::memcmp(string.begin(), "shelterType", 11) ? E::shelterType : E{};
                }
            default:
                switch (string[0]) {
                    case 'f':
                        return string == "funicularType" ? E::funicularType : E{};
                    default:
                        return string == "viewpointType" ? E::viewpointType : E{};
                }
        }
        return E{};
    }
};

struct Color {
    CGFloat r, g, b;

    static constexpr Color black() { return {0, 0, 0}; }
    static constexpr Color white() { return {1, 1, 1}; }

    inline bool operator ==(const Color &other) const {
        return std::tie(r, g, b) == std::tie(other.r, other.g, other.b);
    }
    CGFloat lightness() const { return (r + g + b) / 3; }
};

struct ColorMapping { using E = Color;
    static constexpr DVGKeyPair<E> values[]{ // TODO: hm. are they already sRGB corrected?
        {E{0.0, 0.0, 0}, "black"}, {E{0.3, 0.3, 1}, "blue"},
        {E{0.7, 0.5, 0}, "brown"}, {E{0.2, 0.8, 0.3}, "green"},
        {E{1.0, 0.4, 0}, "orange"}, {E{1, 0.4, 0.4}, "red"},
        {E{0.5, 0, 0.5}, "purple"}, {E{1, 1, 1}, "white"},
        {E{0.8, 0.85, 0}, "yellow"},
        
        // not specified in documentation yet occurs rather frequently:
        {E{0.5, 0.5, 0.5}, "gray"}, {E{0.0, 0.8, 0.8}, "aqua"}, {E{1.0, 0.3, 0.6}, "pink"},
    };
    
    static inline __attribute__((always_inline)) E decode(const std::string_view &string) {
        if (string.size() < 3) { return E{}; }
        switch (string[0]) {
            case 'a':
                return string == "aqua" ? E{0.0, 0.8, 0.8} : E{};
            case 'b':
                switch (string[2]) {
                    case 'a':
                        return string == "black" ? E{0.0, 0.0, 0} : E{};
                    case 'o':
                        return string == "brown" ? E{0.7, 0.5, 0} : E{};
                    default:
                        return string == "blue" ? E{0.3, 0.3, 1} : E{};
                }
            case 'g':
                switch (string[2]) {
                    case 'a':
                        return string == "gray" ? E{0.5, 0.5, 0.5} : E{};
                    default:
                        return string == "green" ? E{0.2, 0.8, 0.3} : E{};
                }
            case 'o':
                return string == "orange" ? E{1.0, 0.4, 0} : E{};
            case 'p':
                switch (string[2]) {
                    case 'n':
                        return string == "pink" ? E{1.0, 0.3, 0.6} : E{};
                    default:
                        return string == "purple" ? E{0.5, 0, 0.5} : E{};
                }
            case 'r':
                return string == "red" ? E{1, 0.4, 0.4} : E{};
            case 'w':
                return string == "white" ? E{1, 1, 1} : E{};
            default:
                return string == "yellow" ? E{0.8, 0.85, 0} : E{};
        }
        return E{};
    }
};

enum class Type { //...[not the full list now]
    unknown = 0, bar, lower, right, stripe,
    corner, pointer, diamond, diamondRight, diamondCorner,
    rectangle, triangle, triangleTurned,
    
    dot, circle, x, cross, triangleLine, diamondLine, rectangleLine,
    // TODO: hiker, shell, modern_shell, etc.
};
struct TypeMapping { using E = Type;
    static constexpr DVGKeyPair<E> values[]{
        {E::bar, "bar"}, {E::lower, "lower"}, {E::right, "right"}, {E::stripe, "stripe"},
        {E::corner, "corner"}, {E::pointer, "pointer"}, {E::diamond, "diamond"},
        {E::diamondRight, "diamond_right"}, {E::diamondCorner, "diamond_corner"},
        {E::rectangle, "rectangle"}, {E::triangle, "triangle"}, {E::triangleTurned, "triangle_turned"},
        
        {E::dot, "dot"}, {E::circle, "circle"}, {E::x, "x"}, {E::cross, "cross"},
        {E::triangleLine, "triangle_line"}, {E::diamondLine, "diamond_line"},
        {E::lower, "rectangle_lower"}, // yes, non-official, yet appears 40 times in USA only
    };
    
    static inline __attribute__((always_inline)) E decode(const std::string_view &string) {
        if (string.size() < 1) { return E{}; }
        switch (string.size()) {
            case 1:
                return !std::memcmp(string.begin(), "x", 1) ? E::x : E{};
            case 3:
                switch (string[0]) {
                    case 'b':
                        return !std::memcmp(string.begin(), "bar", 3) ? E::bar : E{};
                    default:
                        return !std::memcmp(string.begin(), "dot", 3) ? E::dot : E{};
                }
            case 5:
                switch (string[0]) {
                    case 'c':
                        return !std::memcmp(string.begin(), "cross", 5) ? E::cross : E{};
                    case 'l':
                        return !std::memcmp(string.begin(), "lower", 5) ? E::lower : E{};
                    default:
                        return !std::memcmp(string.begin(), "right", 5) ? E::right : E{};
                }
            case 6:
                switch (string[0]) {
                    case 'c':
                        if (!std::memcmp(string.begin(), "corner", 6)) return E::corner;
                        if (!std::memcmp(string.begin(), "circle", 6)) return E::circle;
                        return E{};
                    default:
                        return !std::memcmp(string.begin(), "stripe", 6) ? E::stripe : E{};
                }
            case 7:
                switch (string[0]) {
                    case 'd':
                        return !std::memcmp(string.begin(), "diamond", 7) ? E::diamond : E{};
                    default:
                        return !std::memcmp(string.begin(), "pointer", 7) ? E::pointer : E{};
                }
            case 8:
                return !std::memcmp(string.begin(), "triangle", 8) ? E::triangle : E{};
            case 9:
                return !std::memcmp(string.begin(), "rectangle", 9) ? E::rectangle : E{};
            case 12:
                return !std::memcmp(string.begin(), "diamond_line", 12) ? E::diamondLine : E{};
            case 13:
                switch (string[0]) {
                    case 'd':
                        return !std::memcmp(string.begin(), "diamond_right", 13) ? E::diamondRight : E{};
                    default:
                        return !std::memcmp(string.begin(), "triangle_line", 13) ? E::triangleLine : E{};
                }
            case 14:
                return !std::memcmp(string.begin(), "diamond_corner", 14) ? E::diamondCorner : E{};
            default:
                switch (string[0]) {
                    case 'r':
                        return string == "rectangle_lower" ? E::lower : E{};
                    default:
                        return string == "triangle_turned" ? E::triangleTurned : E{};
                }
        }
        return E{};
    }
};

struct DVGGeoResource {
    enum class Type : uint16_t {
        kUnknown = 0,
        kDEM,
        kHDDEM,
        kTEX,
        kGDB,
        kTRL,

        kHDSAT,

        kSAT,
        kLSAT,
        kLTEX,
        kLDEM,

        kHDWSAT,
        kWSAT,
        kLWSAT,

        kRESOURCE_TYPE_COUNT
    };

    enum class Format : uint16_t {
        kEmpty,
        kRaw,
        kM2SEP,
        kJPEG2000,
        kDFRAC,
        kGDB,
        kTRL,
        kHEIC
    };
};

struct ResourceFormatMapping {
    using E = DVGGeoResource::Format;
    static constexpr DVGKeyPair<E> values[]{
        {E::kEmpty, "EMPTY"},
        {E::kRaw, "RAW"},
        {E::kM2SEP, "M2SEP"},
        {E::kJPEG2000, "J2K"},
        {E::kDFRAC, "DFRAC"},
        {E::kGDB, "GDB"},
        {E::kTRL, "TRL"},
        {E::kHEIC, "HEIC"},
    };
    
    static inline __attribute__((always_inline)) E decode(const std::string_view &string) {
        if (string.size() < 3) { return E{}; }
        switch (string[0]) {
            case 'D':
                return string == "DFRAC" ? E::kDFRAC : E{};
            case 'E':
                return string == "EMPTY" ? E::kEmpty : E{};
            case 'G':
                return string == "GDB" ? E::kGDB : E{};
            case 'H':
                return string == "HEIC" ? E::kHEIC : E{};
            case 'J':
                return string == "J2K" ? E::kJPEG2000 : E{};
            case 'M':
                return string == "M2SEP" ? E::kM2SEP : E{};
            case 'R':
                return string == "RAW" ? E::kRaw : E{};
            default:
                return string == "TRL" ? E::kTRL : E{};
        }
        return E{};
    }
};

struct ResourceTypeMapping {
    using E = DVGGeoResource::Type;
    static constexpr DVGKeyPair<E> values[]{
        {DVGGeoResource::Type::kDEM, "DEM"},
        {DVGGeoResource::Type::kHDDEM, "HDDEM"},
        {DVGGeoResource::Type::kTEX, "TEX"},
        {DVGGeoResource::Type::kGDB, "GDB"},
        {DVGGeoResource::Type::kTRL, "TRL"},

        {DVGGeoResource::Type::kHDSAT, "HDSAT"},

        {DVGGeoResource::Type::kSAT, "SAT"},
        {DVGGeoResource::Type::kLSAT, "LSAT"},
        {DVGGeoResource::Type::kLTEX, "LTEX"},
        {DVGGeoResource::Type::kLDEM, "LDEM"},

        {DVGGeoResource::Type::kHDWSAT, "HDWSAT"},
        {DVGGeoResource::Type::kWSAT, "WSAT"},
        {DVGGeoResource::Type::kLWSAT, "LWSAT"},
    };

    static inline __attribute__((always_inline)) E decode(const std::string_view &string) {
        if (string.size() < 3) { return E{}; }
        switch (string[0]) {
            case 'D':
                return string == "DEM" ? DVGGeoResource::Type::kDEM : E{};
            case 'G':
                return string == "GDB" ? DVGGeoResource::Type::kGDB : E{};
            case 'H':
                if (string == "HDDEM") return DVGGeoResource::Type::kHDDEM;
                if (string == "HDSAT") return DVGGeoResource::Type::kHDSAT;
                if (string == "HDWSAT") return DVGGeoResource::Type::kHDWSAT;
                return E{};
            case 'L':
                switch (string[1]) {
                    case 'D':
                        return string == "LDEM" ? DVGGeoResource::Type::kLDEM : E{};
                    case 'S':
                        return string == "LSAT" ? DVGGeoResource::Type::kLSAT : E{};
                    case 'T':
                        return string == "LTEX" ? DVGGeoResource::Type::kLTEX : E{};
                    default:
                        return string == "LWSAT" ? DVGGeoResource::Type::kLWSAT : E{};
                }
            case 'S':
                return string == "SAT" ? DVGGeoResource::Type::kSAT : E{};
            case 'T':
                switch (string[1]) {
                    case 'E':
                        return string == "TEX" ? DVGGeoResource::Type::kTEX : E{};
                    default:
                        return string == "TRL" ? DVGGeoResource::Type::kTRL : E{};
                }
            default:
                return string == "WSAT" ? DVGGeoResource::Type::kWSAT : E{};
        }
        return E{};
    }
};

TEST_MAPPING(PeakCategory)
TEST_MAPPING(Legacy_EntryType)
TEST_MAPPING(EntryType)
TEST_MAPPING(FunicularType)
TEST_MAPPING(Tags)
TEST_MAPPING(Color)
TEST_MAPPING(Type)
TEST_MAPPING(ResourceFormat)
TEST_MAPPING(ResourceType)

} // namespace
