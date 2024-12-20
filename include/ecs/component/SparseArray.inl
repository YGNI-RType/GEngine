/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** SparseArray.inl
*/

#pragma once
#include "Manager.hpp"

namespace ecs::component {

template <class Component>
void SparseArray<Component>::insert(entity::Entity entity, const Component &component) {
    auto it = m_sparse.find(entity);
    if (it == m_sparse.end()) {
        m_sparse[entity] = m_dense.size();
        m_dense.emplace_back(entity, component);
    } else {
        m_dense[it->second].second = component;
    }
}

template <class Component>
template <typename... Params>
void SparseArray<Component>::emplace(entity::Entity entity, Params &&...p) {
    insert(entity, Component(std::forward<Params>(p)...));
}

template <class Component>
void component::SparseArray<Component>::erase(entity::Entity entity) {
    auto it = m_sparse.find(entity);
    if (it == m_sparse.end())
        return;

    entity::Entity denseIndex = it->second;
    entity::Entity lastDenseIndex = m_dense.size() - 1;
    if (denseIndex != lastDenseIndex) {
        std::swap(m_dense[denseIndex], m_dense[lastDenseIndex]);
        m_sparse[m_dense[denseIndex].first] = denseIndex;
    }
    m_dense.pop_back();
    m_sparse.erase(entity);
}

template <class Component>
bool SparseArray<Component>::contains(entity::Entity entity) const {
    return m_sparse.find(entity) != m_sparse.end();
}

template <class Component>
Component &SparseArray<Component>::get(entity::Entity entity) {
    return m_dense[m_sparse.find(entity)->second].second;
}

template <class Component>
const Component &SparseArray<Component>::get(entity::Entity entity) const {
    return m_dense[m_sparse.find(entity)->second].second;
}

template <class Component>
std::size_t SparseArray<Component>::size() const {
    return m_dense.size();
}

template <class Component>
typename SparseArray<Component>::dense_iterator SparseArray<Component>::begin() {
    return m_dense.begin();
}
template <class Component>
typename SparseArray<Component>::dense_iterator SparseArray<Component>::end() {
    return m_dense.end();
}
template <class Component>
typename SparseArray<Component>::dense_const_iterator SparseArray<Component>::cbegin() const {
    return m_dense.cbegin();
}
template <class Component>
typename SparseArray<Component>::dense_const_iterator SparseArray<Component>::cend() const {
    return m_dense.cend();
}
} // namespace ecs::component