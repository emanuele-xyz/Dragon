#include <Dragon/pch.h>
#include <Dragon/ProjectSettings.h>
#include <Dragon/Commons.h>

#include <pugixml.hpp>

namespace Dragon
{
    ProjectSettings::ProjectSettings(const std::string& path, bool load_from_file)
        : m_path{ path }
        , m_asset_names{}
        , m_scene_names{}
    {
        if (load_from_file)
        {
            auto file_path{ std::format("{}\\{}", path, DRAGON_PROJECT_FILE_NAME) };

            pugi::xml_document doc{};
            pugi::xml_parse_result parse_result{ doc.load_file(file_path.c_str()) };
            Dragon_Check(parse_result);

            pugi::xml_node root{ doc.child("dragon") };
            Dragon_Check(root);

            {
                pugi::xml_node assets{ root.child("assets") };
                Dragon_Check(assets);

                std::unordered_set<std::string> names{};

                for (auto asset{ assets.first_child() }; asset; asset = asset.next_sibling())
                {
                    Dragon_Check(std::strcmp(asset.name(), "asset") == 0);
                    Dragon_Check(!names.contains(asset.name()));

                    m_asset_names.emplace_back(asset.text().as_string());
                    names.emplace(asset.text().as_string());
                }
            }

            {
                pugi::xml_node scenes{ root.child("scenes") };
                Dragon_Check(scenes);

                std::unordered_set<std::string> names{};

                for (auto scene{ scenes.first_child() }; scene; scene = scene.next_sibling())
                {
                    Dragon_Check(std::strcmp(scene.name(), "scene") == 0);
                    Dragon_Check(!names.contains(scene.name()));

                    m_scene_names.emplace_back(scene.text().as_string());
                    names.emplace(scene.text().as_string());
                }
            }
        }
    }

    void ProjectSettings::Save()
    {
        pugi::xml_document doc{};

        pugi::xml_node root{ doc.append_child("dragon") };

        pugi::xml_node assets{ root.append_child("assets") };
        for (const auto& asset_name : m_asset_names)
        {
            pugi::xml_node asset{ assets.append_child("asset") };
            asset.text() = asset_name.c_str();
        }

        pugi::xml_node scenes{ root.append_child("scenes") };
        for (const auto& scene_name : m_scene_names)
        {
            pugi::xml_node scene{ scenes.append_child("scenes") };
            scene.text() = scene_name.c_str();
        }

        auto file_path{ std::format("{}\\{}", m_path, DRAGON_PROJECT_FILE_NAME) };
        Dragon_Check(doc.save_file(file_path.c_str()));
    }
}