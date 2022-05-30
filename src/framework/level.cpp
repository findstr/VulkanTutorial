#include <assert.h>
#include "level.h"
#include "framework/components/meshfilter.h"
#include "framework/components/meshrender.h"
#include "resource/resource.h"

namespace engine {

level::level(const std::string &path)
{
	name = path;
}
level::~level()
{
	for (auto &iter :  gobjects)
		delete iter.second;
}

void
level::tick(float delta)
{
	gobjects.reserve(gobjects.size() + adding.size());
	for (auto &iter:gobjects) {
		auto *go = iter.second;
		auto *p = go->get_parent();
		if (p == nullptr)
			continue;
		auto &trans = go->transform;
		auto &ptrans = p->transform;
		trans.position = ptrans.position + trans.local_position;
		trans.rotation = ptrans.rotation * trans.local_rotation;
		trans.scale = ptrans.scale * trans.local_scale;
	}
	for (auto &iter:gobjects)
		iter.second->tick(delta);
	for (auto &iter:adding) {
		int parent = std::get<int>(iter);
		auto *go = std::get<gameobject *>(iter);
		if (parent != 0) {
			auto p = gobjects[parent];
			go->set_parent(p);
		}
		go->start();
		auto ret = gobjects.insert({go->id(), go});
		assert(ret.second == true);
	}
	for (auto &iter : adding) {
		auto *go = std::get<gameobject *>(iter);
		go->tick(delta);
	}
	adding.clear();
}

void
level::add_gameobject(gameobject *go, int parent)
{
	adding.emplace_back(go, parent);
}

//////---- static level

std::vector<std::unique_ptr<level>> level::levels;

void
level::load(const std::string &path)
{
	level *lv = new level(path);
	levels.emplace_back(lv);
	resource::load_level(path, [&lv](gameobject *go, int parent) {
		lv->add_gameobject(go, parent);
	});
}

void
level::cull(camera *cam, std::vector<draw_object> &list)
{
	for (auto &lv:levels) {
		for (auto &iter:lv->gobjects) {
			auto *go = iter.second;
			auto mf = (meshfilter *)go->get_component("meshfilter");
			auto mr = (meshrender *)go->get_component("meshrender");
			if (mf == nullptr || mr == nullptr)
				continue;
			auto *mesh = mf->get_mesh();
			assert(mesh != nullptr);
			auto *material = mr->get_material();
			list.emplace_back(go->transform, mesh, material);
		}
	}
}

void
level::tick_all(float delta)
{
	for (auto &lv:levels)
		lv->tick(delta);
}

}

