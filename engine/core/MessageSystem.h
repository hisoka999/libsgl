/*
 * MessageSystem.h
 *
 *  Created on: 17.01.2016
 *      Author: stefan
 */

#ifndef CORE_MESSAGESYSTEM_H_
#define CORE_MESSAGESYSTEM_H_

#include <any>
#include <memory>
#include <engine/core/Consumer.h>
#include <engine/core/Message.h>
#include <engine/core/dispatcher.h>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <mutex>

struct MessageDispatcher
{
    core::dispatcher_type dispatcher;
    int reference;
};

static std::mutex g_MessageSystemMutex;

namespace core
{
    template <typename Type>
    struct MessageData
    {
        std::any data;
        Type type;
    };

    template <typename Type>
    class MessageSystem
    {
    public:
        MessageSystem()
            : reference_count(0)
        {
        }
        virtual ~MessageSystem()
        {
            _consumer.clear();
        }

        static MessageSystem &get()
        {
            static MessageSystem instance;
            return instance;
        }
        void deregister(int reference)
        {
            std::lock_guard guard(g_MessageSystemMutex);
            for (auto it = _consumer.begin(); it != _consumer.end(); ++it)
            {
                if (it->second.reference == reference)
                {
                    _consumer.erase(it);
                    return;
                }
            }
        }

        template <typename F>
        [[nodiscard]] int registerForType(Type pType, F &&f)

        {
            std::lock_guard guard(g_MessageSystemMutex);
            reference_count++;
            MessageDispatcher disp;
            disp.dispatcher = core::make_dispatcher(std::forward<F>(f));
            disp.reference = reference_count;
            _consumer.emplace(pType, disp);
            return disp.reference;
        }
        template <typename Data>
        void sendMessage(const std::shared_ptr<Message<Type, Data>> pMessage)
        {
            std::lock_guard guard(g_MessageSystemMutex);
            MessageData<Type> data;
            data.data = pMessage->getData();
            data.type = pMessage->getType();
            _messageQueue.push_back(data);
        }

        void processMessages()
        {
            std::lock_guard guard(g_MessageSystemMutex);
            for (auto &message : _messageQueue)
            {
                auto rng = _consumer.equal_range(message.type);
                for (auto it = rng.first; it != rng.second; ++it)
                {
                    call(it->second.dispatcher, message.data);
                }
            }
            _messageQueue.clear();
        }

    private:
        template <typename F, typename... Args>
        void call(F const &f, Args const &...args)
        {
            std::vector<std::any> v{args...};
            f(v);
        }
        std::unordered_multimap<Type, MessageDispatcher> _consumer;
        std::vector<MessageData<Type>> _messageQueue;

        int reference_count;
    };

} /* namespace core */

#endif /* CORE_MESSAGESYSTEM_H_ */
