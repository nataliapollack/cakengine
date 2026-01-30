#pragma once

// this probably isn't the best way but it's all i got rn.
// keeping it simple and stupid bleh
class Tooling
{
public:
    void register_components();
    void set_system_signatures();

    void serialize_entities();
    void deserialize_entities();

private:
    bool toggle;
};