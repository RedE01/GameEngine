#pragma once
#include <vector>
#include <unordered_map>
#include <cstdint>
#include <cassert>

template <typename T>
class ResourceArray {
public:
	using IDtype = uint32_t;
	using DataStorageType = std::vector<T>;
	using IndexMapType = std::unordered_map<IDtype, size_t>;
	using ReverseIndexType = std::unordered_map<size_t, IDtype>;
	using Iterator = DataStorageType::iterator;

public:
	T* create() {
		m_data.push_back(T(m_nextID));
		size_t dataIndex = m_data.size() - 1;
		m_indexMap[m_nextID] = dataIndex;
		m_reverseIndexMap[dataIndex] = m_nextID;

		++m_nextID;
		return &m_data.back();
	}

	bool remove(IDtype id) {
		// Get the index of the element in m_data
		IndexMapType::iterator search = m_indexMap.find(id);
		if(search == m_indexMap.end()) return false;
		size_t dataIndex = search->second;

		// Remove element from m_indexMap
		m_indexMap.erase(search);

		// If the dataIndex is >= m_data.size() then something has gone wrong
		assert(dataIndex < m_data.size());

		// Copy the last element to the position of the element that is to be removed, and then remove the last element
		m_data[dataIndex] = m_data.back();
		m_data.pop_back();

		// Find the id of the element that was just moved
		ReverseIndexType::iterator search2 = m_reverseIndexMap.find(m_data.size());
		assert(search2 != m_reverseIndexMap.end());
		IDtype otherID = search2->second;

		// Remove the element that was moved from m_reverseIndexMap
		m_reverseIndexMap.erase(search2);

		// If the element that was moved is the same as the one that was deleted (the last element was deleted), then we are done
		if(otherID == id) return true;

		// Otherwise, we have to update m_indexMap and m_reverseIndexMap for the element that was moved
		m_indexMap[otherID] = dataIndex;
		m_reverseIndexMap[dataIndex] = otherID;
		
		return true;
	}

	size_t size() const { return m_data.size(); }

	Iterator begin() { return m_data.begin(); }
	Iterator end() { return m_data.end(); }

private:
	DataStorageType m_data;
	IndexMapType m_indexMap;
	ReverseIndexType m_reverseIndexMap;
	IDtype m_nextID = 0;
};