class Entity{
    public:
    enum class Type {
        Paddle,
        Brick,
        Bullet,
        Ground
    };
    
    Type entity_type;
    Entity(Type type);
    Type getEntityType();
    void setEntityType(Type type);
};