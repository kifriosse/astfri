class Vec2
{
    float x_dim_;
    float y_dim_;
public:
    void normalize();
    Vec2 add(Vec2 p_other);
    float dot_product(Vec2 p_other);
    float magnitude();
};

class Vec3
{
    float x_dim_;
    float y_dim_;
    float z_dim_;
public:
    void normalize();
    Vec3 add(Vec3 p_other);
    float dot_product(Vec3 p_other);
    float magnitude();
};

class Renderer{};
class VulkanRenderer : public Renderer{};
class DirectXRenderer : public Renderer{};
class MetalRenderer : public Renderer{};
class OpenGLRenderer : public Renderer{};

class AudioChannel
{
    int channel_id_;
};

class AudioClip
{
    char* path_;
};

class AudioEffect{};

class AudioEngine
{
private:
    int main_channel_volume_;
    int music_channel_volume_;
    int dialogue_channel_volume_;
    int sfx_channel_volume_;
public:
    void play_global_audio(AudioClip p_audio, AudioChannel p_channel);
    void apply_effect(AudioEffect p_effect, AudioChannel p_channel);
};

class AudioEngine2D : public AudioEngine
{
public:
    void play_local_audio(AudioClip p_audio, AudioChannel p_channel, Vec2 p_pos);
};

class AudioEngine3D : public AudioEngine
{
public:
    void play_local_audio(AudioClip p_audio, AudioChannel p_channel, Vec3 p_pos);
};

class Asset
{
    char* path_;
};

class AssetLoader
{
public:
    Asset load_asset_2D(char* p_path);
    Asset load_asset_3D(char* p_path);
};

class EntityType{};

class Entity
{
    EntityType type_;
};

class Entity2D : public Entity
{
    Vec2 position_;
    Vec2 velocity_;
    float rotation_;
};

class Entity3D : public Entity
{
    Vec3 position_;
    Vec3 velocity_;
    Vec3 rotation_;
};

class Collider
{
    bool interacts_with_static_;
    bool interacts_with_mobile_;
    Entity entity_;
};

class PhysicsEngine
{
protected:
    float grav_accel_;
};

class PhysicsEngine2D : public PhysicsEngine
{
    Vec2 grav_dir_;
};

class PhysicsEngine3D : public PhysicsEngine
{
    Vec3 grav_dir_;
};

class GameEngine
{
    PhysicsEngine physics_;
    AudioEngine audio_;
    AssetLoader loader_;
    Renderer render_;
};
