/*
** EPITECH PROJECT, 2024
** GameEngine
** File description:
** SharedEvent.cpp
*/

template <class... Events>
gengine::interface::network::system::ServerEventReceiver<Events...>::ServerEventReceiver()
    : m_server(Network::NET::getServer()) {
}

template <class... Events>
void gengine::interface::network::system::ServerEventReceiver<Events...>::init(void) {
    this->template subscribeToEvent<gengine::system::event::GameLoop>(&ServerEventReceiver::onGameLoop);
    (dynamicPublish<Events>(), ...);
}

template <class... Events>
void gengine::interface::network::system::ServerEventReceiver<Events...>::onGameLoop(
    gengine::system::event::GameLoop &e) {
    auto &clients = this->template getSystem<gengine::interface::network::system::ServerClientsHandler>();
    size_t readCount = 0;

    for (auto &[uuid, client] : clients.getClients()) {
        if (client.shouldDelete())
            continue;

        Network::UDPMessage msg(Network::UDPMessage::HEADER, Network::CL_EVENT);
        if (!client.getNet()->popIncommingData(msg, readCount))
            continue;

        std::uint64_t nb;
        msg.readContinuousData(nb, readCount);

        client.setLastAck(msg.getAckNumber());
        // std::cout << "Last ACK: " << client.getLastAck() << std::endl;
        for (size_t i = 0; i < nb; i++) {
            std::uint64_t type;
            msg.readContinuousData(type, readCount);
            auto it = m_eventsCallbacks.find(type);
            if (it == m_eventsCallbacks.end())
                throw std::runtime_error("Event type not found");

            auto &[callback, size] = it->second;
            std::vector<Network::byte_t> data(size);

            msg.readData(data.data(), readCount, size);
            uuids::uuid id = uuid;
            callback(data.data(), id);
        }
    }
}

template <class... Events>
template <typename T>
void gengine::interface::network::system::ServerEventReceiver<Events...>::dynamicPublish(void) {
    m_eventsCallbacks.insert(std::make_pair(
        m_id, std::make_pair<std::function<void(void *, uuids::uuid &)>, size_t>(
                  [this](void *data, uuids::uuid &remoteUUID) -> void {
                      gengine::interface::event::SharedEvent<T> event(*reinterpret_cast<T *>(data), remoteUUID);
                      this->template publishEvent<gengine::interface::event::SharedEvent<T>>(event);
                  },
                  sizeof(T))));
    m_id++;
}
