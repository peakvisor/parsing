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

    static inline E decode(std::string_view string) {
        switch (string.size()) {
            case 4:
                switch (string[2]) {
                    case 'k':
                        return !std::memcmp(string.begin(), "lake", 4) ? E::kLake : E{};
                    case 's':
                        return !std::memcmp(string.begin(), "pass", 4) ? E::kPass : E{};
                    default:
                        return !std::memcmp(string.begin(), "peak", 4) ? E::kPeak
                            : E{}; //case 'a'
                }
            case 5:
                return !std::memcmp(string.begin(), "place", 5) ? E::kPlace : E{};
            case 6:
                return !std::memcmp(string.begin(), "castle", 6) ? E::kCastle : E{};
            case 7:
                switch (string[2]) {
                    case 'e':
                        return !std::memcmp(string.begin(), "amenity", 7) ? E::kAmenity
                            : E{};
                    case 'r':
                        return !std::memcmp(string.begin(), "parking", 7) ? E::kParking
                            : E{};
                    default:
                        return !std::memcmp(string.begin(), "glacier", 7) ? E::kGlacier
                            : E{}; //case 'a'
                }
            default:
                switch (string[2]) {
                    case 'e':
                        return !std::memcmp(string.begin(), "viewpoint", 8) ? E::kViewpoint : E{};
                    case 'n':
                        return string == "funicularStation" ? E::kFunicular : E{};
                    case 't':
                        return !std::memcmp(string.begin(), "waterfall", 9) ? E::kWaterfall : E{};
                    default:
                        return !std::memcmp(string.begin(), "alpineHut", 8) ? E::kMountainHut : E{};
                }
        }
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

    static inline E decode(std::string_view string) {
        using T = ResourceTypeMapping::E;
        if (string.size() >= 3) {
            switch (string[2]) {
                case 'A': return string[0] == 'L' ?
                        (string == "LSAT" ? T::kLSAT : T{}):
                        string == "WSAT" ? T::kWSAT : T{};
                case 'B': return string == "GDB" ? T::kGDB : T{};
                case 'D': return string == "HDDEM" ? T::kHDDEM : T{};
                case 'E': return string[1] == 'T' ?
                        (string == "LTEX" ? T::kLTEX : T{}):
                        string == "LDEM" ? T::kLDEM : T{};
                case 'L': return string == "TRL" ? T::kTRL : T{};
                case 'M': return string == "DEM" ? T::kDEM : T{};
                case 'S': return string[1] == 'D' ?
                        (string == "HDSAT" ? T::kHDSAT : T{}) :
                        string == "LWSAT" ? T::kLWSAT : T{};
                case 'T': return string == "SAT" ? T::kSAT : T{};
                case 'W': return string == "HDWSAT" ? T::kHDWSAT : T{};
                default: return string == "TEX" ? T::kTEX : T{};
            }
        }
        return {};
    }
};
