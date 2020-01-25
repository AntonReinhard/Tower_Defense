// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: map.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_map_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_map_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3010000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3010001 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/map.h>  // IWYU pragma: export
#include <google/protobuf/map_entry.h>
#include <google/protobuf/map_field_inl.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_map_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_map_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxillaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[3]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_map_2eproto;
namespace towerdefense {
class map;
class mapDefaultTypeInternal;
extern mapDefaultTypeInternal _map_default_instance_;
class map_Layer;
class map_LayerDefaultTypeInternal;
extern map_LayerDefaultTypeInternal _map_Layer_default_instance_;
class map_TileMapEntry_DoNotUse;
class map_TileMapEntry_DoNotUseDefaultTypeInternal;
extern map_TileMapEntry_DoNotUseDefaultTypeInternal _map_TileMapEntry_DoNotUse_default_instance_;
}  // namespace towerdefense
PROTOBUF_NAMESPACE_OPEN
template<> ::towerdefense::map* Arena::CreateMaybeMessage<::towerdefense::map>(Arena*);
template<> ::towerdefense::map_Layer* Arena::CreateMaybeMessage<::towerdefense::map_Layer>(Arena*);
template<> ::towerdefense::map_TileMapEntry_DoNotUse* Arena::CreateMaybeMessage<::towerdefense::map_TileMapEntry_DoNotUse>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace towerdefense {

// ===================================================================

class map_Layer :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:towerdefense.map.Layer) */ {
 public:
  map_Layer();
  virtual ~map_Layer();

  map_Layer(const map_Layer& from);
  map_Layer(map_Layer&& from) noexcept
    : map_Layer() {
    *this = ::std::move(from);
  }

  inline map_Layer& operator=(const map_Layer& from) {
    CopyFrom(from);
    return *this;
  }
  inline map_Layer& operator=(map_Layer&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return GetMetadataStatic().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return GetMetadataStatic().reflection;
  }
  static const map_Layer& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const map_Layer* internal_default_instance() {
    return reinterpret_cast<const map_Layer*>(
               &_map_Layer_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(map_Layer& a, map_Layer& b) {
    a.Swap(&b);
  }
  inline void Swap(map_Layer* other) {
    if (other == this) return;
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline map_Layer* New() const final {
    return CreateMaybeMessage<map_Layer>(nullptr);
  }

  map_Layer* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<map_Layer>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const map_Layer& from);
  void MergeFrom(const map_Layer& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* InternalSerializeWithCachedSizesToArray(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(map_Layer* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "towerdefense.map.Layer";
  }
  private:
  inline ::PROTOBUF_NAMESPACE_ID::Arena* GetArenaNoVirtual() const {
    return nullptr;
  }
  inline void* MaybeArenaPtr() const {
    return nullptr;
  }
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_map_2eproto);
    return ::descriptor_table_map_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kTilesFieldNumber = 1,
  };
  // repeated uint32 tiles = 1;
  int tiles_size() const;
  private:
  int _internal_tiles_size() const;
  public:
  void clear_tiles();
  private:
  ::PROTOBUF_NAMESPACE_ID::uint32 _internal_tiles(int index) const;
  const ::PROTOBUF_NAMESPACE_ID::RepeatedField< ::PROTOBUF_NAMESPACE_ID::uint32 >&
      _internal_tiles() const;
  void _internal_add_tiles(::PROTOBUF_NAMESPACE_ID::uint32 value);
  ::PROTOBUF_NAMESPACE_ID::RepeatedField< ::PROTOBUF_NAMESPACE_ID::uint32 >*
      _internal_mutable_tiles();
  public:
  ::PROTOBUF_NAMESPACE_ID::uint32 tiles(int index) const;
  void set_tiles(int index, ::PROTOBUF_NAMESPACE_ID::uint32 value);
  void add_tiles(::PROTOBUF_NAMESPACE_ID::uint32 value);
  const ::PROTOBUF_NAMESPACE_ID::RepeatedField< ::PROTOBUF_NAMESPACE_ID::uint32 >&
      tiles() const;
  ::PROTOBUF_NAMESPACE_ID::RepeatedField< ::PROTOBUF_NAMESPACE_ID::uint32 >*
      mutable_tiles();

  // @@protoc_insertion_point(class_scope:towerdefense.map.Layer)
 private:
  class _Internal;

  ::PROTOBUF_NAMESPACE_ID::internal::InternalMetadataWithArena _internal_metadata_;
  ::PROTOBUF_NAMESPACE_ID::RepeatedField< ::PROTOBUF_NAMESPACE_ID::uint32 > tiles_;
  mutable std::atomic<int> _tiles_cached_byte_size_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_map_2eproto;
};
// -------------------------------------------------------------------

class map_TileMapEntry_DoNotUse : public ::PROTOBUF_NAMESPACE_ID::internal::MapEntry<map_TileMapEntry_DoNotUse, 
    ::PROTOBUF_NAMESPACE_ID::uint32, std::string,
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::TYPE_UINT32,
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::TYPE_STRING,
    0 > {
public:
  typedef ::PROTOBUF_NAMESPACE_ID::internal::MapEntry<map_TileMapEntry_DoNotUse, 
    ::PROTOBUF_NAMESPACE_ID::uint32, std::string,
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::TYPE_UINT32,
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::TYPE_STRING,
    0 > SuperType;
  map_TileMapEntry_DoNotUse();
  map_TileMapEntry_DoNotUse(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  void MergeFrom(const map_TileMapEntry_DoNotUse& other);
  static const map_TileMapEntry_DoNotUse* internal_default_instance() { return reinterpret_cast<const map_TileMapEntry_DoNotUse*>(&_map_TileMapEntry_DoNotUse_default_instance_); }
  static bool ValidateKey(void*) { return true; }
  static bool ValidateValue(std::string* s) {
    return ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(s->data(), s->size(), ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::PARSE, "towerdefense.map.TileMapEntry.value");
 }
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& other) final;
  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_map_2eproto);
    return ::descriptor_table_map_2eproto.file_level_metadata[1];
  }

  public:
};

// -------------------------------------------------------------------

class map :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:towerdefense.map) */ {
 public:
  map();
  virtual ~map();

  map(const map& from);
  map(map&& from) noexcept
    : map() {
    *this = ::std::move(from);
  }

  inline map& operator=(const map& from) {
    CopyFrom(from);
    return *this;
  }
  inline map& operator=(map&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return GetMetadataStatic().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return GetMetadataStatic().reflection;
  }
  static const map& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const map* internal_default_instance() {
    return reinterpret_cast<const map*>(
               &_map_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    2;

  friend void swap(map& a, map& b) {
    a.Swap(&b);
  }
  inline void Swap(map* other) {
    if (other == this) return;
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline map* New() const final {
    return CreateMaybeMessage<map>(nullptr);
  }

  map* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<map>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const map& from);
  void MergeFrom(const map& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* InternalSerializeWithCachedSizesToArray(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(map* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "towerdefense.map";
  }
  private:
  inline ::PROTOBUF_NAMESPACE_ID::Arena* GetArenaNoVirtual() const {
    return nullptr;
  }
  inline void* MaybeArenaPtr() const {
    return nullptr;
  }
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_map_2eproto);
    return ::descriptor_table_map_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  typedef map_Layer Layer;

  // accessors -------------------------------------------------------

  enum : int {
    kLayersFieldNumber = 1,
    kTileMapFieldNumber = 3,
    kResourcesFieldNumber = 2,
    kWidthFieldNumber = 4,
    kHeightFieldNumber = 5,
  };
  // repeated .towerdefense.map.Layer layers = 1;
  int layers_size() const;
  private:
  int _internal_layers_size() const;
  public:
  void clear_layers();
  ::towerdefense::map_Layer* mutable_layers(int index);
  ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::towerdefense::map_Layer >*
      mutable_layers();
  private:
  const ::towerdefense::map_Layer& _internal_layers(int index) const;
  ::towerdefense::map_Layer* _internal_add_layers();
  public:
  const ::towerdefense::map_Layer& layers(int index) const;
  ::towerdefense::map_Layer* add_layers();
  const ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::towerdefense::map_Layer >&
      layers() const;

  // map<uint32, string> tile_map = 3;
  int tile_map_size() const;
  private:
  int _internal_tile_map_size() const;
  public:
  void clear_tile_map();
  private:
  const ::PROTOBUF_NAMESPACE_ID::Map< ::PROTOBUF_NAMESPACE_ID::uint32, std::string >&
      _internal_tile_map() const;
  ::PROTOBUF_NAMESPACE_ID::Map< ::PROTOBUF_NAMESPACE_ID::uint32, std::string >*
      _internal_mutable_tile_map();
  public:
  const ::PROTOBUF_NAMESPACE_ID::Map< ::PROTOBUF_NAMESPACE_ID::uint32, std::string >&
      tile_map() const;
  ::PROTOBUF_NAMESPACE_ID::Map< ::PROTOBUF_NAMESPACE_ID::uint32, std::string >*
      mutable_tile_map();

  // .towerdefense.map.Layer resources = 2;
  bool has_resources() const;
  private:
  bool _internal_has_resources() const;
  public:
  void clear_resources();
  const ::towerdefense::map_Layer& resources() const;
  ::towerdefense::map_Layer* release_resources();
  ::towerdefense::map_Layer* mutable_resources();
  void set_allocated_resources(::towerdefense::map_Layer* resources);
  private:
  const ::towerdefense::map_Layer& _internal_resources() const;
  ::towerdefense::map_Layer* _internal_mutable_resources();
  public:

  // uint32 width = 4;
  void clear_width();
  ::PROTOBUF_NAMESPACE_ID::uint32 width() const;
  void set_width(::PROTOBUF_NAMESPACE_ID::uint32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint32 _internal_width() const;
  void _internal_set_width(::PROTOBUF_NAMESPACE_ID::uint32 value);
  public:

  // uint32 height = 5;
  void clear_height();
  ::PROTOBUF_NAMESPACE_ID::uint32 height() const;
  void set_height(::PROTOBUF_NAMESPACE_ID::uint32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint32 _internal_height() const;
  void _internal_set_height(::PROTOBUF_NAMESPACE_ID::uint32 value);
  public:

  // @@protoc_insertion_point(class_scope:towerdefense.map)
 private:
  class _Internal;

  ::PROTOBUF_NAMESPACE_ID::internal::InternalMetadataWithArena _internal_metadata_;
  ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::towerdefense::map_Layer > layers_;
  ::PROTOBUF_NAMESPACE_ID::internal::MapField<
      map_TileMapEntry_DoNotUse,
      ::PROTOBUF_NAMESPACE_ID::uint32, std::string,
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::TYPE_UINT32,
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::TYPE_STRING,
      0 > tile_map_;
  ::towerdefense::map_Layer* resources_;
  ::PROTOBUF_NAMESPACE_ID::uint32 width_;
  ::PROTOBUF_NAMESPACE_ID::uint32 height_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_map_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// map_Layer

// repeated uint32 tiles = 1;
inline int map_Layer::_internal_tiles_size() const {
  return tiles_.size();
}
inline int map_Layer::tiles_size() const {
  return _internal_tiles_size();
}
inline void map_Layer::clear_tiles() {
  tiles_.Clear();
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 map_Layer::_internal_tiles(int index) const {
  return tiles_.Get(index);
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 map_Layer::tiles(int index) const {
  // @@protoc_insertion_point(field_get:towerdefense.map.Layer.tiles)
  return _internal_tiles(index);
}
inline void map_Layer::set_tiles(int index, ::PROTOBUF_NAMESPACE_ID::uint32 value) {
  tiles_.Set(index, value);
  // @@protoc_insertion_point(field_set:towerdefense.map.Layer.tiles)
}
inline void map_Layer::_internal_add_tiles(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  tiles_.Add(value);
}
inline void map_Layer::add_tiles(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  _internal_add_tiles(value);
  // @@protoc_insertion_point(field_add:towerdefense.map.Layer.tiles)
}
inline const ::PROTOBUF_NAMESPACE_ID::RepeatedField< ::PROTOBUF_NAMESPACE_ID::uint32 >&
map_Layer::_internal_tiles() const {
  return tiles_;
}
inline const ::PROTOBUF_NAMESPACE_ID::RepeatedField< ::PROTOBUF_NAMESPACE_ID::uint32 >&
map_Layer::tiles() const {
  // @@protoc_insertion_point(field_list:towerdefense.map.Layer.tiles)
  return _internal_tiles();
}
inline ::PROTOBUF_NAMESPACE_ID::RepeatedField< ::PROTOBUF_NAMESPACE_ID::uint32 >*
map_Layer::_internal_mutable_tiles() {
  return &tiles_;
}
inline ::PROTOBUF_NAMESPACE_ID::RepeatedField< ::PROTOBUF_NAMESPACE_ID::uint32 >*
map_Layer::mutable_tiles() {
  // @@protoc_insertion_point(field_mutable_list:towerdefense.map.Layer.tiles)
  return _internal_mutable_tiles();
}

// -------------------------------------------------------------------

// -------------------------------------------------------------------

// map

// repeated .towerdefense.map.Layer layers = 1;
inline int map::_internal_layers_size() const {
  return layers_.size();
}
inline int map::layers_size() const {
  return _internal_layers_size();
}
inline void map::clear_layers() {
  layers_.Clear();
}
inline ::towerdefense::map_Layer* map::mutable_layers(int index) {
  // @@protoc_insertion_point(field_mutable:towerdefense.map.layers)
  return layers_.Mutable(index);
}
inline ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::towerdefense::map_Layer >*
map::mutable_layers() {
  // @@protoc_insertion_point(field_mutable_list:towerdefense.map.layers)
  return &layers_;
}
inline const ::towerdefense::map_Layer& map::_internal_layers(int index) const {
  return layers_.Get(index);
}
inline const ::towerdefense::map_Layer& map::layers(int index) const {
  // @@protoc_insertion_point(field_get:towerdefense.map.layers)
  return _internal_layers(index);
}
inline ::towerdefense::map_Layer* map::_internal_add_layers() {
  return layers_.Add();
}
inline ::towerdefense::map_Layer* map::add_layers() {
  // @@protoc_insertion_point(field_add:towerdefense.map.layers)
  return _internal_add_layers();
}
inline const ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::towerdefense::map_Layer >&
map::layers() const {
  // @@protoc_insertion_point(field_list:towerdefense.map.layers)
  return layers_;
}

// .towerdefense.map.Layer resources = 2;
inline bool map::_internal_has_resources() const {
  return this != internal_default_instance() && resources_ != nullptr;
}
inline bool map::has_resources() const {
  return _internal_has_resources();
}
inline void map::clear_resources() {
  if (GetArenaNoVirtual() == nullptr && resources_ != nullptr) {
    delete resources_;
  }
  resources_ = nullptr;
}
inline const ::towerdefense::map_Layer& map::_internal_resources() const {
  const ::towerdefense::map_Layer* p = resources_;
  return p != nullptr ? *p : *reinterpret_cast<const ::towerdefense::map_Layer*>(
      &::towerdefense::_map_Layer_default_instance_);
}
inline const ::towerdefense::map_Layer& map::resources() const {
  // @@protoc_insertion_point(field_get:towerdefense.map.resources)
  return _internal_resources();
}
inline ::towerdefense::map_Layer* map::release_resources() {
  // @@protoc_insertion_point(field_release:towerdefense.map.resources)
  
  ::towerdefense::map_Layer* temp = resources_;
  resources_ = nullptr;
  return temp;
}
inline ::towerdefense::map_Layer* map::_internal_mutable_resources() {
  
  if (resources_ == nullptr) {
    auto* p = CreateMaybeMessage<::towerdefense::map_Layer>(GetArenaNoVirtual());
    resources_ = p;
  }
  return resources_;
}
inline ::towerdefense::map_Layer* map::mutable_resources() {
  // @@protoc_insertion_point(field_mutable:towerdefense.map.resources)
  return _internal_mutable_resources();
}
inline void map::set_allocated_resources(::towerdefense::map_Layer* resources) {
  ::PROTOBUF_NAMESPACE_ID::Arena* message_arena = GetArenaNoVirtual();
  if (message_arena == nullptr) {
    delete resources_;
  }
  if (resources) {
    ::PROTOBUF_NAMESPACE_ID::Arena* submessage_arena = nullptr;
    if (message_arena != submessage_arena) {
      resources = ::PROTOBUF_NAMESPACE_ID::internal::GetOwnedMessage(
          message_arena, resources, submessage_arena);
    }
    
  } else {
    
  }
  resources_ = resources;
  // @@protoc_insertion_point(field_set_allocated:towerdefense.map.resources)
}

// map<uint32, string> tile_map = 3;
inline int map::_internal_tile_map_size() const {
  return tile_map_.size();
}
inline int map::tile_map_size() const {
  return _internal_tile_map_size();
}
inline void map::clear_tile_map() {
  tile_map_.Clear();
}
inline const ::PROTOBUF_NAMESPACE_ID::Map< ::PROTOBUF_NAMESPACE_ID::uint32, std::string >&
map::_internal_tile_map() const {
  return tile_map_.GetMap();
}
inline const ::PROTOBUF_NAMESPACE_ID::Map< ::PROTOBUF_NAMESPACE_ID::uint32, std::string >&
map::tile_map() const {
  // @@protoc_insertion_point(field_map:towerdefense.map.tile_map)
  return _internal_tile_map();
}
inline ::PROTOBUF_NAMESPACE_ID::Map< ::PROTOBUF_NAMESPACE_ID::uint32, std::string >*
map::_internal_mutable_tile_map() {
  return tile_map_.MutableMap();
}
inline ::PROTOBUF_NAMESPACE_ID::Map< ::PROTOBUF_NAMESPACE_ID::uint32, std::string >*
map::mutable_tile_map() {
  // @@protoc_insertion_point(field_mutable_map:towerdefense.map.tile_map)
  return _internal_mutable_tile_map();
}

// uint32 width = 4;
inline void map::clear_width() {
  width_ = 0u;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 map::_internal_width() const {
  return width_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 map::width() const {
  // @@protoc_insertion_point(field_get:towerdefense.map.width)
  return _internal_width();
}
inline void map::_internal_set_width(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  
  width_ = value;
}
inline void map::set_width(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  _internal_set_width(value);
  // @@protoc_insertion_point(field_set:towerdefense.map.width)
}

// uint32 height = 5;
inline void map::clear_height() {
  height_ = 0u;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 map::_internal_height() const {
  return height_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 map::height() const {
  // @@protoc_insertion_point(field_get:towerdefense.map.height)
  return _internal_height();
}
inline void map::_internal_set_height(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  
  height_ = value;
}
inline void map::set_height(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  _internal_set_height(value);
  // @@protoc_insertion_point(field_set:towerdefense.map.height)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------

// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace towerdefense

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_map_2eproto
