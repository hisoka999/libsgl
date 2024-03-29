/*
 * Message.h
 *
 *  Created on: 17.01.2016
 *      Author: stefan
 */

#ifndef CORE_MESSAGE_H_
#define CORE_MESSAGE_H_

namespace core
{

    template<typename T, typename Data>
    class Message final
    {
    public:
        Message(const T &type, Data data) : _type(type), _data(data) {}
        ~Message() = default;
        T getType() { return _type; }
        Data getData() { return _data; }

    protected:
        T _type;
        Data _data;
    };

} /* namespace core */

#endif /* CORE_MESSAGE_H_ */
