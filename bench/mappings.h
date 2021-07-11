namespace generated_mappings {

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
        switch(string.size()) {
            case 4:
                switch(string[1]) {
                    case '3':
                        return !std::memcmp(string.begin(), "13er", 4) ? E::k13er : E{};
                    default: 
                        return !std::memcmp(string.begin(), "14er", 4) ? E::k14er : E{};
                }
            case 5:
                switch(string[4]) {
                    case '0':
                        switch(string[3]) {
                            case '0':
                                switch(string[1]) {
                                    case 'A':
                                        return !std::memcmp(string.begin(), "JA100", 5) ? E::ja100 : E{};
                                    default: 
                                        return !std::memcmp(string.begin(), "TW100", 5) ? E::tw100 : E{};
                                }
                            case '1':
                                return !std::memcmp(string.begin(), "Top10", 5) ? E::top10 : E{};
                            case '4':
                                return !std::memcmp(string.begin(), "TGO40", 5) ? E::tgo40 : E{};
                            default: 
                                return !std::memcmp(string.begin(), "Top50", 5) ? E::top50 : E{};
                        }
                    case '6':
                        return !std::memcmp(string.begin(), "AHP46", 5) ? E::ahp46 : E{};
                    case 'a':
                        return !std::memcmp(string.begin(), "Ultra", 5) ? E::ultra : E{};
                    case 'h':
                        return !std::memcmp(string.begin(), "Furth", 5) ? E::furth : E{};
                    default: 
                        return !std::memcmp(string.begin(), "Munro", 5) ? E::munro : E{};
                }
            case 7:
                switch(string[6]) {
                    case 'o':
                        return !std::memcmp(string.begin(), "Volcano", 7) ? E::volcano : E{};
                    default: 
                        return !std::memcmp(string.begin(), "Glacier", 7) ? E::glacier : E{};
                }
            default: return E{};
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
        switch(string.size()) {
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
            case 17:
                return !std::memcmp(string.begin(), "funicular_station", 17) ? E::kFunicular : E{};
            default: return E{};
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
        switch(string.size()) {
            case 4:
                switch(string[3]) {
                    case 'e':
                        return !std::memcmp(string.begin(), "lake", 4) ? E::kLake : E{};
                    case 'k':
                        return !std::memcmp(string.begin(), "peak", 4) ? E::kPeak : E{};
                    default: 
                        return !std::memcmp(string.begin(), "pass", 4) ? E::kPass : E{};
                }
            case 5:
                return !std::memcmp(string.begin(), "place", 5) ? E::kPlace : E{};
            case 6:
                return !std::memcmp(string.begin(), "castle", 6) ? E::kCastle : E{};
            case 7:
                switch(string[6]) {
                    case 'g':
                        return !std::memcmp(string.begin(), "parking", 7) ? E::kParking : E{};
                    case 'r':
                        return !std::memcmp(string.begin(), "glacier", 7) ? E::kGlacier : E{};
                    default: 
                        return !std::memcmp(string.begin(), "amenity", 7) ? E::kAmenity : E{};
                }
            case 9:
                switch(string[8]) {
                    case 'l':
                        return !std::memcmp(string.begin(), "waterfall", 9) ? E::kWaterfall : E{};
                    default: 
                        switch(string[7]) {
                            case 'n':
                                return !std::memcmp(string.begin(), "viewpoint", 9) ? E::kViewpoint : E{};
                            default: 
                                return !std::memcmp(string.begin(), "alpineHut", 9) ? E::kMountainHut : E{};
                        }
                }
            case 16:
                return !std::memcmp(string.begin(), "funicularStation", 16) ? E::kFunicular : E{};
            default: return E{};
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
        switch(string.size()) {
            case 4:
                return !std::memcmp(string.begin(), "rail", 4) ? E::kDVGTrailTypeNarrowGauge : E{};
            case 5:
                switch(string[0]) {
                    case 'j':
                        return !std::memcmp(string.begin(), "j-bar", 5) ? E::kDVGTrailTypeDragLift : E{};
                    default: 
                        return !std::memcmp(string.begin(), "t-bar", 5) ? E::kDVGTrailTypeDragLift : E{};
                }
            case 7:
                switch(string[6]) {
                    case 'a':
                        return !std::memcmp(string.begin(), "gondola", 7) ? E::kDVGTrailTypeGondola : E{};
                    default: 
                        return !std::memcmp(string.begin(), "platter", 7) ? E::kDVGTrailTypeDragLift : E{};
                }
            case 9:
                switch(string[8]) {
                    case 'r':
                        switch(string[6]) {
                            case 'c':
                                return !std::memcmp(string.begin(), "cable_car", 9) ? E::kDVGTrailTypeCableCar : E{};
                            default: 
                                return !std::memcmp(string.begin(), "funicular", 9) ? E::kDVGTrailTypeFunicular : E{};
                        }
                    default: 
                        return !std::memcmp(string.begin(), "drag_lift", 9) ? E::kDVGTrailTypeDragLift : E{};
                }
            case 10:
                return !std::memcmp(string.begin(), "chair_lift", 10) ? E::kDVGTrailTypeChairLift : E{};
            case 12:
                switch(string[11]) {
                    case 'e':
                        return !std::memcmp(string.begin(), "narrow_gauge", 12) ? E::kDVGTrailTypeNarrowGauge : E{};
                    default: 
                        return !std::memcmp(string.begin(), "magic_carpet", 12) ? E::kDVGTrailTypeMagicCarpet : E{};
                }
            default: return E{};
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
        switch(string.size()) {
            case 3:
                return !std::memcmp(string.begin(), "url", 3) ? E::url : E{};
            case 4:
                return !std::memcmp(string.begin(), "area", 4) ? E::area : E{};
            case 5:
                return !std::memcmp(string.begin(), "phone", 5) ? E::phone : E{};
            case 6:
                return !std::memcmp(string.begin(), "height", 6) ? E::height : E{};
            case 7:
                switch(string[6]) {
                    case 'e':
                        return !std::memcmp(string.begin(), "hutType", 7) ? E::hutType : E{};
                    case 'l':
                        return !std::memcmp(string.begin(), "natural", 7) ? E::natural : E{};
                    default: 
                        return !std::memcmp(string.begin(), "amenity", 7) ? E::amenity : E{};
                }
            case 8:
                switch(string[7]) {
                    case 'g':
                        return !std::memcmp(string.begin(), "drinking", 8) ? E::drinking : E{};
                    default: 
                        switch(string[6]) {
                            case 'r':
                                return !std::memcmp(string.begin(), "category", 8) ? E::peakCategory : E{};
                            default: 
                                return !std::memcmp(string.begin(), "capacity", 8) ? E::capacity : E{};
                        }
                }
            case 9:
                return !std::memcmp(string.begin(), "relatedID", 9) ? E::relatedID : E{};
            case 10:
                return !std::memcmp(string.begin(), "population", 10) ? E::population : E{};
            case 11:
                switch(string[6]) {
                    case 'r':
                        return !std::memcmp(string.begin(), "shelterType", 11) ? E::shelterType : E{};
                    default: 
                        return !std::memcmp(string.begin(), "amenityType", 11) ? E::amenityType : E{};
                }
            case 13:
                switch(string[8]) {
                    case 'r':
                        return !std::memcmp(string.begin(), "funicularType", 13) ? E::funicularType : E{};
                    default: 
                        return !std::memcmp(string.begin(), "viewpointType", 13) ? E::viewpointType : E{};
                }
            default: return E{};
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
        switch(string.size()) {
            case 3:
                return !std::memcmp(string.begin(), "red", 3) ? E{1, 0.4, 0.4} : E{};
            case 4:
                switch(string[3]) {
                    case 'a':
                        return !std::memcmp(string.begin(), "aqua", 4) ? E{0.0, 0.8, 0.8} : E{};
                    case 'e':
                        return !std::memcmp(string.begin(), "blue", 4) ? E{0.3, 0.3, 1} : E{};
                    case 'k':
                        return !std::memcmp(string.begin(), "pink", 4) ? E{1.0, 0.3, 0.6} : E{};
                    default: 
                        return !std::memcmp(string.begin(), "gray", 4) ? E{0.5, 0.5, 0.5} : E{};
                }
            case 5:
                switch(string[4]) {
                    case 'e':
                        return !std::memcmp(string.begin(), "white", 5) ? E{1, 1, 1} : E{};
                    case 'k':
                        return !std::memcmp(string.begin(), "black", 5) ? E{0.0, 0.0, 0} : E{};
                    default: 
                        switch(string[3]) {
                            case 'e':
                                return !std::memcmp(string.begin(), "green", 5) ? E{0.2, 0.8, 0.3} : E{};
                            default: 
                                return !std::memcmp(string.begin(), "brown", 5) ? E{0.7, 0.5, 0} : E{};
                        }
                }
            case 6:
                switch(string[5]) {
                    case 'e':
                        switch(string[4]) {
                            case 'g':
                                return !std::memcmp(string.begin(), "orange", 6) ? E{1.0, 0.4, 0} : E{};
                            default: 
                                return !std::memcmp(string.begin(), "purple", 6) ? E{0.5, 0, 0.5} : E{};
                        }
                    default: 
                        return !std::memcmp(string.begin(), "yellow", 6) ? E{0.8, 0.85, 0} : E{};
                }
            default: return E{};
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
        switch(string.size()) {
            case 1:
                return !std::memcmp(string.begin(), "x", 1) ? E::x : E{};
            case 3:
                switch(string[2]) {
                    case 'r':
                        return !std::memcmp(string.begin(), "bar", 3) ? E::bar : E{};
                    default: 
                        return !std::memcmp(string.begin(), "dot", 3) ? E::dot : E{};
                }
            case 5:
                switch(string[4]) {
                    case 'r':
                        return !std::memcmp(string.begin(), "lower", 5) ? E::lower : E{};
                    case 's':
                        return !std::memcmp(string.begin(), "cross", 5) ? E::cross : E{};
                    default: 
                        return !std::memcmp(string.begin(), "right", 5) ? E::right : E{};
                }
            case 6:
                switch(string[5]) {
                    case 'e':
                        switch(string[4]) {
                            case 'l':
                                return !std::memcmp(string.begin(), "circle", 6) ? E::circle : E{};
                            default: 
                                return !std::memcmp(string.begin(), "stripe", 6) ? E::stripe : E{};
                        }
                    default: 
                        return !std::memcmp(string.begin(), "corner", 6) ? E::corner : E{};
                }
            case 7:
                switch(string[6]) {
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
                switch(string[12]) {
                    case 'e':
                        return !std::memcmp(string.begin(), "triangle_line", 13) ? E::triangleLine : E{};
                    default: 
                        return !std::memcmp(string.begin(), "diamond_right", 13) ? E::diamondRight : E{};
                }
            case 14:
                return !std::memcmp(string.begin(), "diamond_corner", 14) ? E::diamondCorner : E{};
            case 15:
                switch(string[14]) {
                    case 'd':
                        return !std::memcmp(string.begin(), "triangle_turned", 15) ? E::triangleTurned : E{};
                    default: 
                        return !std::memcmp(string.begin(), "rectangle_lower", 15) ? E::lower : E{};
                }
            default: return E{};
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
        switch(string.size()) {
            case 3:
                switch(string[2]) {
                    case 'B':
                        return !std::memcmp(string.begin(), "GDB", 3) ? E::kGDB : E{};
                    case 'K':
                        return !std::memcmp(string.begin(), "J2K", 3) ? E::kJPEG2000 : E{};
                    case 'L':
                        return !std::memcmp(string.begin(), "TRL", 3) ? E::kTRL : E{};
                    default: 
                        return !std::memcmp(string.begin(), "RAW", 3) ? E::kRaw : E{};
                }
            case 4:
                return !std::memcmp(string.begin(), "HEIC", 4) ? E::kHEIC : E{};
            case 5:
                switch(string[4]) {
                    case 'C':
                        return !std::memcmp(string.begin(), "DFRAC", 5) ? E::kDFRAC : E{};
                    case 'P':
                        return !std::memcmp(string.begin(), "M2SEP", 5) ? E::kM2SEP : E{};
                    default: 
                        return !std::memcmp(string.begin(), "EMPTY", 5) ? E::kEmpty : E{};
                }
            default: return E{};
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
        switch(string.size()) {
            case 3:
                switch(string[2]) {
                    case 'B':
                        return !std::memcmp(string.begin(), "GDB", 3) ? DVGGeoResource::Type::kGDB : E{};
                    case 'L':
                        return !std::memcmp(string.begin(), "TRL", 3) ? DVGGeoResource::Type::kTRL : E{};
                    case 'M':
                        return !std::memcmp(string.begin(), "DEM", 3) ? DVGGeoResource::Type::kDEM : E{};
                    case 'T':
                        return !std::memcmp(string.begin(), "SAT", 3) ? DVGGeoResource::Type::kSAT : E{};
                    default: 
                        return !std::memcmp(string.begin(), "TEX", 3) ? DVGGeoResource::Type::kTEX : E{};
                }
            case 4:
                switch(string[3]) {
                    case 'M':
                        return !std::memcmp(string.begin(), "LDEM", 4) ? DVGGeoResource::Type::kLDEM : E{};
                    case 'T':
                        switch(string[0]) {
                            case 'L':
                                return !std::memcmp(string.begin(), "LSAT", 4) ? DVGGeoResource::Type::kLSAT : E{};
                            default: 
                                return !std::memcmp(string.begin(), "WSAT", 4) ? DVGGeoResource::Type::kWSAT : E{};
                        }
                    default: 
                        return !std::memcmp(string.begin(), "LTEX", 4) ? DVGGeoResource::Type::kLTEX : E{};
                }
            case 5:
                switch(string[4]) {
                    case 'M':
                        return !std::memcmp(string.begin(), "HDDEM", 5) ? DVGGeoResource::Type::kHDDEM : E{};
                    default: 
                        switch(string[1]) {
                            case 'D':
                                return !std::memcmp(string.begin(), "HDSAT", 5) ? DVGGeoResource::Type::kHDSAT : E{};
                            default: 
                                return !std::memcmp(string.begin(), "LWSAT", 5) ? DVGGeoResource::Type::kLWSAT : E{};
                        }
                }
            case 6:
                return !std::memcmp(string.begin(), "HDWSAT", 6) ? DVGGeoResource::Type::kHDWSAT : E{};
            default: return E{};
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
