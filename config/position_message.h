#ifndef AR_POSITION_MESSAGE_H
#define AR_POSITION_MESSAGE_H

#include <string>
#include <sstream>

struct Point3D
{
    int x;
    int y;
    int z;

    Point3D &operator+=(const Point3D &other)
    {
        this->x += other.x;
        this->y += other.y;
        this->z += other.z;
        return *this;
    }
};

struct PositionMessage
{
    int id;
    Point3D position;

    // Serialize the struct to a string
    std::string serialize() const
    {
        std::stringstream ss;
        ss << "ID: " << id
           << ", X: " << position.x
           << ", Y: " << position.y
           << ", Z: " << position.z;
        return ss.str();
    }

    // Deserialize a string into a PositionMessage struct
    static PositionMessage deserialize(const std::string &serializedData)
    {
        PositionMessage message{};
        std::istringstream ss(serializedData);

        // Extract values directly based on the known format
        std::string prefix;
        ss >> prefix >> message.id;
        ss.ignore(2);
        ss >> prefix >> message.position.x;
        ss.ignore(2);
        ss >> prefix >> message.position.y;
        ss.ignore(2);
        ss >> prefix >> message.position.z;

        return message;
    }
};

#endif // AR_POSITION_MESSAGE_H
