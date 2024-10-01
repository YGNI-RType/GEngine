/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet (Workspace)
** File description:
** net
*/

#include "GEngine/cvar/net.hpp"

namespace CVar {
ConVar net_ipv6("net_ipv6", "0", ConVar::flag::CMD, "Enable ipv6 (set in convar)");

ConVar sv_maxplayers("sv_maxplayers", "4", ConVar::flag::SERVER | ConVar::flag::SETUP, "Set the maxmium number of players a server can accept");

ConVar net_recv_dropsim("net_recv_dropsim", "0", ConVar::flag::CLIENT, "Simulate recv packet drop rate (%)");
ConVar net_send_dropsim("net_send_dropsim", "0", ConVar::flag::CLIENT, "Simulate send packet drop rate (%)");
} // namespace CVar
