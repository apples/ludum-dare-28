#ifndef SCREEN_HPP
#define SCREEN_HPP

class Screen
{
public:
    struct Event
    {
        enum Type
        {
            NONE
            , PUSH
            , POP
            , SWAP
        };

        Type type;
        Screen* value;
    };

    virtual ~Screen() = 0;

    virtual bool isOpaque() const = 0;
    virtual bool isTunnel() const = 0;

    virtual Event tick() = 0;
    virtual void draw() = 0;
};

#endif // SCREEN_HPP
