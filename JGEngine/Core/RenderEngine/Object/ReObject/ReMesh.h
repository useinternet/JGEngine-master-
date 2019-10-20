#pragma once


#include "Object/ReObject.h"
#include <vector>
namespace RE
{
	class CommandList;
	class ReMesh : public ReObject
	{
		using VertexData = std::vector<byte>;
		using IndexData  = std::vector<uint32_t>;
	public:
		ReMesh() : ReObject("ReMesh") {}
		ReMesh(const std::string& name) : ReObject(name) {}
	public:
		template<typename T>
		void Set(const std::vector<T>& v, const IndexData& i)
		{
			m_VertexTypeSize = (uint32_t)sizeof(T);
			size_t btSize = v.size() * sizeof(T);
			m_MeshVertexDatas.resize(btSize);
			memcpy_s(&m_MeshVertexDatas[0], btSize, v.data(), btSize);
			m_MeshIndexDatas  = std::move(i);
		}

		uint32_t GetTypeSize() const {
			return m_VertexTypeSize;
		}

		const VertexData& GetVertexData() const {
			return m_MeshVertexDatas;
		}
		const IndexData& GetIndexData() const {
			return m_MeshIndexDatas;
		}
		void SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY topology) {
			m_Topology = topology;
		}
		void Draw(CommandList* cmdList, uint32_t instanceCount = 1);
	private:
		VertexData   m_MeshVertexDatas;
		IndexData    m_MeshIndexDatas;
		uint32_t     m_VertexTypeSize;
		D3D_PRIMITIVE_TOPOLOGY m_Topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	public:
		static ReMesh Merge(const ReMesh& m1, const ReMesh& m2);
	};
}