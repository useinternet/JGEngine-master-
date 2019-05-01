#include<PCH.h>
#include"Gui_DirectoryViewer.h"
#include<Data/Path.h>
#include<Data/Log.h>
#include"ImGui_Custom/DirectoryTree.h"
using namespace JgGui;
using namespace std;

REISTER_COMMONPATH

Gui_DirectoryViewer::Gui_DirectoryViewer(const std::string& name) 
	: GuiComponent(name)
{
	fs::directory_iterator directoryIter(GLOBAL_PATH / "Contents\\");
	m_RootNode = CreateDirectoryNode();
	m_RootNode->IsDirectroy = true;
	m_RootNode->name = "Contents";
	m_RootNode->path = GLOBAL_PATH / "Contents\\";

	SearchRecursiveDirectoryNode(directoryIter, m_RootNode);
}
Gui_DirectoryViewer::~Gui_DirectoryViewer()
{

}
void Gui_DirectoryViewer::NewFrame()
{
	if (m_IsVisible)
	{
		GuiComponent::NewFrame();

	
		ImGui::Begin(m_GuiName.c_str(), &m_IsVisible, 
			ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoScrollWithMouse);

		float window_width = ImGui::GetWindowWidth();
		float window_height = ImGui::GetWindowHeight();

		// �޴�
		if (ImGui::BeginMenuBar())
		{
			OnGui_MainMenu();
			ImGui::EndMenuBar();
		}
		//
		ImGui::Columns(2);
		if (m_FirstFrame)
		{
			ImGui::SetColumnWidth(0, window_width * 0.3f);
			ImGui::SetColumnWidth(1, window_width * 0.7f);
			m_FirstFrame = false;
		}
		float blank = 10.0f;
		float child_height = window_height - 50.0f;



		// ���丮 ��� �� �˻� �ڽ� ������
		{
			ImGui::BeginChild(
				"DirectorySearch",
				{ ImGui::GetColumnWidth() - blank, child_height },
				true, ImGuiWindowFlags_AlwaysVerticalScrollbar);

			OnGui_DirectroySearchWindow(m_RootNode);

			ImGui::EndChild();
		}

		ImGui::NextColumn();


		// ���� ��� ������ 
		{
			ImGui::BeginChild(
				"FileViwer",
				{ ImGui::GetColumnWidth() - blank, child_height },
				true, ImGuiWindowFlags_AlwaysVerticalScrollbar);

			OnGui_FileViewerWindow();

			ImGui::EndChild();

		}
		ImGui::End();
	}
}
void Gui_DirectoryViewer::OnGui_MainMenu()
{
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("Copy", nullptr, nullptr))
		{

		}
		if (ImGui::MenuItem("Cut", nullptr, nullptr))
		{

		}
		if (ImGui::MenuItem("Paste", nullptr, nullptr))
		{

		}
		ImGui::EndMenu();
	}
}
void Gui_DirectoryViewer::OnGui_DirectroySearchWindow(DirectoryNode* CurrDirNode)
{
	if (!CurrDirNode->IsDirectroy)
		return;

	// ��ư ���� 
	{
		//  ���� ���丮 ��忡 ���� ���丮�� ���ٸ� ��ư ���� X
		if (CurrDirNode->DirectoryCount == 0)
		{
			ImGui::Dummy({ m_DirNodeItemSize ,m_DirNodeItemSize });
		}
		// ���� ���丮 ������ Open ��ư ����
		else
		{
			ImVec4 button_color;
			(CurrDirNode->IsOpen) ?
				button_color = m_Dir_Open_Color :
				button_color = m_Dir_Close_Color;

			if (ImGui::ColorButton(
				("##" + CurrDirNode->name).c_str(),
				button_color,
				ImGuiColorEditFlags_Float,
				{ m_DirNodeItemSize,m_DirNodeItemSize }))
			{
				(CurrDirNode->IsOpen) ?
					CurrDirNode->IsOpen = false :
					CurrDirNode->IsOpen = true;
			}
		}

	}

	ImGui::SameLine();

	// ���丮 ���
	{
		ImGui::Selectable(CurrDirNode->name.c_str(), &CurrDirNode->IsClicked,
			ImGuiSelectableFlags_AllowDoubleClick);
		m_DirNodeItemSize = ImGui::GetItemRectSize().y;


		// ���� ��� ���丮 ��尡 Ŭ���Ǿ��ٸ�
		if (CurrDirNode->IsClicked)
		{
			// case 1 :  ���� Ŭ������ ��尡 ������
			if (m_Curr_Clicked_Node == nullptr)
			{
				m_Curr_Clicked_Node = CurrDirNode;
			}
			// case 2 : ���� ���� ���� Ŭ���� ����� ���� ��ġ ���� ������ 
			// ���� Ŭ���� ��� ����
			else if (CurrDirNode != m_Curr_Clicked_Node)
			{
				m_Prev_Clicked_Node = m_Curr_Clicked_Node;
				m_Curr_Clicked_Node = CurrDirNode;
				m_Prev_Clicked_Node->IsClicked = false;
			}
		}
		// ���� ��� ���丮 ��尡 �ٽ� Ŭ���Ǿ� Ŭ���� ��� �Ǿ��ٸ�
		// �Լ� Ŭ���� ���·� ����
		else
		{
			if (m_Curr_Clicked_Node == CurrDirNode)
			{
				m_Curr_Clicked_Node->IsClicked = true;
			}
		}

		// ���� ��� ���丮 ��尡 ����Ŭ���ҽ� ���� ��� ���� ����
		if (ImGui::IsItemClicked() && ImGui::IsMouseDoubleClicked(0))
		{
			m_Selected_Node = CurrDirNode;
			ENGINE_LOG("Gui_DirectoryViewer", Common::Log::Info, m_Selected_Node->name.c_str());
		}

		// ���� ��ư�� ���� ���丮�� �������� ���� ���丮 �׸���.
		if (CurrDirNode->IsOpen)
		{
			ImGui::Indent(10.0f);
			for (auto& node : CurrDirNode->datas)
			{
				OnGui_DirectroySearchWindow(node);
			}
			ImGui::Unindent(10.0f);
		}

	}
	
}
void Gui_DirectoryViewer::OnGui_FileViewerWindow()
{
	if (m_Selected_Node == nullptr)
		return;

	std::vector<DirectoryNode*> dirFile;
	std::vector<DirectoryNode*> etcFile;

	ImGui::BeginGroup();

	for (auto& file : m_Selected_Node->datas)
	{
		if (file->IsDirectroy)
		{
			ImGui::Text(file->name.c_str());
		}
		else
		{
			ImGui::Text(file->name.c_str());
		}
	}


	ImGui::EndGroup();
}
Gui_DirectoryViewer::DirectoryNode* Gui_DirectoryViewer::CreateDirectoryNode()
{
	auto d = make_unique<DirectoryNode>();
	DirectoryNode* result = d.get();
	m_DirectroyNodeMems.push_back(move(d));
	return result;
}
void Gui_DirectoryViewer::SearchRecursiveDirectoryNode(
	const fs::directory_iterator& directoryIter,
	DirectoryNode* directoryNode)
{

	for (auto& file : directoryIter)
	{
		DirectoryNode* node = CreateDirectoryNode();
		node->name = file.path().filename().string();
		node->path = file.path().string();
		node->parent = directoryNode;
		directoryNode->datas.push_back(node);

		if (fs::is_directory(file.status()))
		{
			directoryNode->DirectoryCount++;
			node->IsDirectroy = true;
			SearchRecursiveDirectoryNode(
				fs::directory_iterator(file.path()), node);
		}
	}
}