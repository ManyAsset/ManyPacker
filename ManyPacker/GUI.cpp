#include "GUI.hpp"
#include "imgui.h"
#include "utils.hpp"
#include "defines.hpp"
#include "prefs.hpp"
#include "assets.hpp"

namespace ManyPacker
{
	namespace GUI
	{
		void DrawGUI()
		{
			static bool about = false;
			static bool showSelectDir = true;

            SetStyle();
            ImGui::NewFrame();

            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->Pos);
            ImGui::SetNextWindowSize(viewport->Size);
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar
                | ImGuiWindowFlags_NoCollapse
                | ImGuiWindowFlags_NoResize
                | ImGuiWindowFlags_NoMove
                | ImGuiWindowFlags_NoBringToFrontOnFocus
                | ImGuiWindowFlags_MenuBar;

            // Window title and basic setup
            ImGui::Begin(MANYPACKER_NAME, nullptr, window_flags);

            float totalWidth = ImGui::GetContentRegionAvail().x;
            float spacing = ImGui::GetStyle().ItemSpacing.x;

            float childWidth = (totalWidth - 2 * spacing) / 3;

            // === Toolbar using ImGui::BeginMenuBar() ===
            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("File"))
                {
                    if (ImGui::MenuItem("Set CoD4 folder..."))
                    {
                        ManyPacker::Utils::selectCoD4Root();
                    }

                    ImGui::Separator();

                    if (ImGui::MenuItem("Exit"))
                        exit(0);

                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("Help"))
                {
                    if (ImGui::MenuItem("About"))
                        about = true;

                    ImGui::EndMenu();
                }

                ImGui::EndMenuBar();
            }

            if(about)
            {
                ImGui::OpenPopup("About");
				about = false;
			}

            if (ManyPacker::Prefs::rootfolder.empty() && showSelectDir)
            {
                ImGui::OpenPopup("CoD4 root dir not found!");
				showSelectDir = false;
            }

            if (!ManyPacker::Utils::checkCod4Dir() && !ManyPacker::Prefs::rootfolder.empty())
                ImGui::OpenPopup("Invalid CoD4 root dir!");

            if (ManyPacker::Assets::exportStatus == 0)
            {
				ImGui::OpenPopup("No Assets Selected!");
				ManyPacker::Assets::exportStatus = -1;
            }
            else if (ManyPacker::Assets::exportStatus == 1)
            {
                ImGui::OpenPopup("Success!");
                ManyPacker::Assets::exportStatus = -1;
            } 
            else if (ManyPacker::Assets::exportStatus == 2)
            {
                ImGui::OpenPopup("Error!");
				ManyPacker::Assets::exportStatus = -1;
            }

			Components::AboutWindow();
			Components::SelectDirWindow();
			Components::InvalidDirWindow();
			Components::SuccessfulExportWindow();
			Components::ErrorExportWindow();
			Components::AssetListEmptyWindow();

            // === Left Panel: Asset Selection ===
            ImGui::BeginChild("AssetSelection", ImVec2(childWidth, 0), true);
            ImGui::Text("Asset Selection");
            ImGui::Separator();

            // Tabs for XModels, SP Weapons, MP Weapons
            if (ImGui::BeginTabBar("##AssetTabs"))
            {
                Components::ItemSelection("XModels", ManyPacker::Utils::XModels, ManyPacker::Utils::AssetType::XMODEL);
                Components::ItemSelection("MP Weapons", ManyPacker::Utils::MPWeapons, ManyPacker::Utils::AssetType::MP_WEAPON);
				Components::ItemSelection("SP Weapons", ManyPacker::Utils::SPWeapons, ManyPacker::Utils::AssetType::SP_WEAPON);

                ImGui::EndTabBar();
            }

            ImGui::EndChild();

            ImGui::SameLine();

            // === Center Panel: Selected Assets ===
            ImGui::BeginChild("SelectedAssets", ImVec2(childWidth, 0), true);
            ImGui::Text("Selected Assets");
            ImGui::Separator();

            static int item_current = -1;
            static bool item_highlight = false;

            if (ImGui::BeginListBox("Assets", ImVec2(300, 300)))
            {
                for (int n = 0; n < ManyPacker::Utils::SelectedAssets.size(); n++)
                {
                    const bool is_selected = (item_current == n);
                    if (ImGui::Selectable((ManyPacker::Utils::SelectedAssets[n].type.toString() + ManyPacker::Utils::SelectedAssets[n].name).c_str(), is_selected))
                        item_current = n;

                    if (item_highlight && ImGui::IsItemHovered())
                        item_current = n;

                    // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndListBox();
            }

			ImGui::SetCursorPosY(368);

            if (ImGui::Button("Remove Selected", ImVec2(-1, 0)))
            {
                ManyPacker::Utils::removeAsset(item_current);
            }

            if (ImGui::Button("Clear List", ImVec2(-1, 0)))
            {
                ManyPacker::Utils::SelectedAssets.clear();
            }

            ImGui::EndChild();

            ImGui::SameLine();

            // === Right Panel: Export & Settings ===
            ImGui::BeginChild("ExportSettings", ImVec2(childWidth, 0), true);
            ImGui::Text("Export");
            ImGui::Separator();

            // Output directory selector (placeholder)
            ImGui::InputText("Output Dir", ManyPacker::Prefs::outputfolder, IM_ARRAYSIZE(ManyPacker::Prefs::outputfolder));
            if (ImGui::Button("Browse"))
            {
				std::wstring selectedFolder = ManyPacker::Utils::selectFolder();

                if (!selectedFolder.empty())
                {
                    wcstombs_s(NULL, ManyPacker::Prefs::outputfolder, sizeof(ManyPacker::Prefs::outputfolder), selectedFolder.c_str(), _TRUNCATE);

                    std::wstring outputFolderWString = std::wstring(ManyPacker::Prefs::outputfolder, ManyPacker::Prefs::outputfolder + strlen(ManyPacker::Prefs::outputfolder));
                    ManyPacker::Prefs::savePreference(outputFolderWString, L"outputfolder");
                }
            }

            ImGui::InputText("Output Name", ManyPacker::Prefs::outputfolderName, IM_ARRAYSIZE(ManyPacker::Prefs::outputfolderName));

            // Export format combo box
            static int exportFormat = 0;
            const char* formats[] = { ".zip", ".rar", ".7z", "Folder - No archive"};
            ImGui::Combo("Format", &exportFormat, formats, IM_ARRAYSIZE(formats));

            ImGui::SetCursorPosY(395);

            // Export button
            if (ImGui::Button("Export", ImVec2(-1, 0)))
            {
                ManyPacker::Assets::ProcessAllAssets();
            }

            ImGui::EndChild();

            ImGui::End();
            ImGui::EndFrame();
		}

        void SetStyle()
        {
            ImGuiStyle& style = ImGui::GetStyle();

            // Layout
            style.Alpha = 1.0f;
            style.DisabledAlpha = 0.6f;
            style.WindowPadding = ImVec2(8, 8);
            //style.WindowRounding = 4.0f;
            style.WindowBorderSize = 1.0f;
            style.WindowMinSize = ImVec2(32, 32);
            style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
            style.FramePadding = ImVec2(6, 4);
            style.FrameRounding = 4.0f;
            style.FrameBorderSize = 0.0f;
            style.ItemSpacing = ImVec2(8, 6);
            style.ItemInnerSpacing = ImVec2(4, 4);
            style.IndentSpacing = 20.0f;
            style.ScrollbarSize = 14.0f;
            style.ScrollbarRounding = 6.0f;
            style.GrabMinSize = 10.0f;
            style.GrabRounding = 4.0f;
            style.TabRounding = 4.0f;
            style.TabBorderSize = 0.0f;

            // Colors
            ImVec4 orange = ImVec4(0.76470588235f, 0.41960784313f, 0.08235294117f, 1.0f); // #ff8d1d
            ImVec4 orangeHover = ImVec4(1.0f, 0.6f, 0.2f, 1.0f);
            ImVec4 orangeActive = ImVec4(0.9f, 0.45f, 0.1f, 1.0f);
            ImVec4 bgDark = ImVec4(0.07f, 0.07f, 0.07f, 0.94f);
            ImVec4 bgLight = ImVec4(0.14f, 0.14f, 0.14f, 1.0f);
            ImVec4 bgMedium = ImVec4(0.12f, 0.12f, 0.12f, 1.0f);
            ImVec4 textColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
            ImVec4 disabledText = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
            ImVec4 borderColor = ImVec4(0.35f, 0.35f, 0.4f, 0.5f);

            ImVec4* colors = style.Colors;

            colors[ImGuiCol_Text] = textColor;
            colors[ImGuiCol_TextDisabled] = disabledText;
            colors[ImGuiCol_WindowBg] = bgDark;
            colors[ImGuiCol_ChildBg] = bgLight;
            colors[ImGuiCol_PopupBg] = bgLight;
            colors[ImGuiCol_Border] = borderColor;
            colors[ImGuiCol_BorderShadow] = ImVec4(0, 0, 0, 0);
            colors[ImGuiCol_FrameBg] = bgMedium;
            colors[ImGuiCol_FrameBgHovered] = orangeHover;
            colors[ImGuiCol_FrameBgActive] = orangeActive;
            colors[ImGuiCol_TitleBg] = bgMedium;
            colors[ImGuiCol_TitleBgActive] = bgLight;
            colors[ImGuiCol_TitleBgCollapsed] = bgMedium;
            colors[ImGuiCol_MenuBarBg] = bgMedium;
            colors[ImGuiCol_ScrollbarBg] = bgDark;
            colors[ImGuiCol_ScrollbarGrab] = orange;
            colors[ImGuiCol_ScrollbarGrabHovered] = orangeHover;
            colors[ImGuiCol_ScrollbarGrabActive] = orangeActive;
            colors[ImGuiCol_CheckMark] = orange;
            colors[ImGuiCol_SliderGrab] = orange;
            colors[ImGuiCol_SliderGrabActive] = orangeActive;
            colors[ImGuiCol_Button] = orange;
            colors[ImGuiCol_ButtonHovered] = orangeHover;
            colors[ImGuiCol_ButtonActive] = orangeActive;
            colors[ImGuiCol_Header] = orange;
            colors[ImGuiCol_HeaderHovered] = orangeHover;
            colors[ImGuiCol_HeaderActive] = orangeActive;
            colors[ImGuiCol_Separator] = borderColor;
            colors[ImGuiCol_SeparatorHovered] = orangeHover;
            colors[ImGuiCol_SeparatorActive] = orangeActive;
            colors[ImGuiCol_ResizeGrip] = orange;
            colors[ImGuiCol_ResizeGripHovered] = orangeHover;
            colors[ImGuiCol_ResizeGripActive] = orangeActive;
            colors[ImGuiCol_Tab] = bgMedium;
            colors[ImGuiCol_TabHovered] = orangeHover;
            colors[ImGuiCol_TabActive] = orange;
            colors[ImGuiCol_TabUnfocused] = bgMedium;
            colors[ImGuiCol_TabUnfocusedActive] = orange;
            colors[ImGuiCol_PlotLines] = orange;
            colors[ImGuiCol_PlotLinesHovered] = orangeHover;
            colors[ImGuiCol_PlotHistogram] = orange;
            colors[ImGuiCol_PlotHistogramHovered] = orangeHover;
            colors[ImGuiCol_TableHeaderBg] = bgLight;
            colors[ImGuiCol_TableBorderStrong] = borderColor;
            colors[ImGuiCol_TableBorderLight] = borderColor;
            colors[ImGuiCol_TableRowBg] = bgDark;
            colors[ImGuiCol_TableRowBgAlt] = bgMedium;
            colors[ImGuiCol_TextSelectedBg] = ImVec4(orange.x, orange.y, orange.z, 0.35f);
            colors[ImGuiCol_DragDropTarget] = orange;
            colors[ImGuiCol_NavHighlight] = orange;
            colors[ImGuiCol_NavWindowingHighlight] = orange;
            colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.8f, 0.8f, 0.8f, 0.2f);
            colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.2f, 0.2f, 0.2f, 0.4f);
        }

	}

    namespace Components
    {
        void AboutWindow()
        {
            ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y / 2), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

            if (ImGui::BeginPopupModal("About", NULL, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::Text("ManyPacker");
                ImGui::Separator();
                ImGui::Text("A simple asset packer that automatically grabs every file connected\nto a specific asset from a CoD4 raw folder.");
                ImGui::Text("Made by: Rex109");

                if (ImGui::Button("OK", ImVec2(120, 0)))
                    ImGui::CloseCurrentPopup();

                ImGui::SetItemDefaultFocus();
                ImGui::EndPopup();
            }
        }

        void SelectDirWindow()
        {
            ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y / 2), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

            if (ImGui::BeginPopupModal("CoD4 root dir not found!", NULL, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::Text("The CoD4 root folder has not been found automatically, please select it manually.");

                if (ImGui::Button("OK", ImVec2(120, 0)))
                {
                    ManyPacker::Utils::selectCoD4Root();
                    ImGui::CloseCurrentPopup();
                }

                ImGui::SetItemDefaultFocus();
                ImGui::EndPopup();
            }
        }

        void InvalidDirWindow()
        {
            ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y / 2), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

            if (ImGui::BeginPopupModal("Invalid CoD4 root dir!", NULL, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::Text("The selected directory is invalid.");

                if (ImGui::Button("OK", ImVec2(120, 0)))
                {
                    ManyPacker::Utils::selectCoD4Root();
                    ImGui::CloseCurrentPopup();
                }

                ImGui::SetItemDefaultFocus();
                ImGui::EndPopup();
            }
        }

        void SuccessfulExportWindow()
        {
            ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y / 2), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

            if (ImGui::BeginPopupModal("Success!", NULL, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::Text("Asset(s) exported successfully.");

                if (ImGui::Button("OK", ImVec2(120, 0)))
                    ImGui::CloseCurrentPopup();

                ImGui::SetItemDefaultFocus();
                ImGui::EndPopup();
            }
        }

        void ErrorExportWindow()
        {
            ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y / 2), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

            if (ImGui::BeginPopupModal("Error!", NULL, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::Text("An error occurred during export.");

                if (ImGui::Button("OK", ImVec2(120, 0)))
                    ImGui::CloseCurrentPopup();

                ImGui::SetItemDefaultFocus();
                ImGui::EndPopup();
			}
        }

        void AssetListEmptyWindow()
        {
            ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y / 2), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

            if (ImGui::BeginPopupModal("No Assets Selected!", NULL, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::Text("Please select at least one asset to export.");

                if (ImGui::Button("OK", ImVec2(120, 0)))
                    ImGui::CloseCurrentPopup();

                ImGui::SetItemDefaultFocus();
                ImGui::EndPopup();
            }
        }

        void ItemSelection(std::string name, std::vector<std::string> &items, ManyPacker::Utils::AssetType::Type type)
        {
            if (ImGui::BeginTabItem(name.c_str()))
            {
                static char filterBuffer[128] = "";
                static int selectedItem = -1;

                ImGui::InputText(("Search##" + name).c_str(), filterBuffer, IM_ARRAYSIZE(filterBuffer));

                static int item_current = 1;

                if (ImGui::BeginListBox(name.c_str(), ImVec2(300, 300)))
                {
                    for (int i = 0; i < items.size(); i++)
                    {
						char lowercaseItem[128];
						strncpy_s(lowercaseItem, items[i].c_str(), sizeof(lowercaseItem));

						for (int j = 0; lowercaseItem[j]; j++)
							lowercaseItem[j] = tolower(lowercaseItem[j]);

						char filterBufferLower[128];

						strncpy_s(filterBufferLower, filterBuffer, sizeof(filterBufferLower));

						for (int j = 0; filterBufferLower[j]; j++)
							filterBufferLower[j] = tolower(filterBufferLower[j]);


                        if (strlen(filterBufferLower) > 0 && strstr(lowercaseItem, filterBufferLower) == nullptr)
                        {
                            continue;
                        }

                        const bool isSelected = (selectedItem == i);
                        if (ImGui::Selectable(items[i].c_str(), isSelected))
                            selectedItem = i;

                        if (isSelected)
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndListBox();
                }
                
				ImGui::SetCursorPosY(395);

                if (ImGui::Button("Add Selected", ImVec2(-1, 0)) && selectedItem != -1)
                {
                    ManyPacker::Utils::addAsset(items[selectedItem], type);

                    if(ManyPacker::Utils::SelectedAssets.size() == 1)
                    {
						strcpy_s(ManyPacker::Prefs::outputfolderName, sizeof(ManyPacker::Prefs::outputfolderName), ManyPacker::Utils::SelectedAssets[0].name.c_str());
					}
                }

                ImGui::EndTabItem();
                
            }   
        }
    }
}