#pragma once
#ifndef ES_CORE_COMPONENTS_MENU_COMPONENT_H
#define ES_CORE_COMPONENTS_MENU_COMPONENT_H

#include "components/ComponentGrid.h"
#include "components/ComponentList.h"
#include "components/NinePatchComponent.h"
#include "components/TextComponent.h"
#include "utils/StringUtil.h"
#include "components/ComponentTab.h"

class ButtonComponent;
class ImageComponent;

std::shared_ptr<ComponentGrid> makeButtonGrid(Window* window, const std::vector< std::shared_ptr<ButtonComponent> >& buttons);
std::shared_ptr<ImageComponent> makeArrow(Window* window);

#define TITLE_VERT_PADDING (Renderer::getScreenHeight()*0.0637f)
#define TITLE_WITHSUB_VERT_PADDING (Renderer::getScreenHeight()*0.05f)
#define SUBTITLE_VERT_PADDING (Renderer::getScreenHeight()*0.019f)

class MenuComponent : public GuiComponent
{
public:
	MenuComponent(Window* window, 
		const std::string& title, const std::shared_ptr<Font>& titleFont = Font::get(FONT_SIZE_LARGE),
		const std::string& subTitle = "", bool tabbedUI = true);

	void onSizeChanged() override;

	inline void setUpdateType(ComponentListFlags::UpdateType updateType) { mList->setUpdateType(updateType); }

	inline void addRow(const ComponentListRow& row, bool setCursorHere = false, bool doUpdateSize = true, const std::string& userData = "") { mList->addRow(row, setCursorHere, true, userData); if (doUpdateSize) updateSize(); }
	inline void clear() { mList->clear(); }

	void addWithLabel(const std::string& label, const std::shared_ptr<GuiComponent>& comp, const std::function<void()>& func = nullptr, const std::string& iconName = "", bool setCursorHere = false);
	void addWithDescription(const std::string& label, const std::string& description, const std::shared_ptr<GuiComponent>& comp, const std::function<void()>& func = nullptr, const std::string& iconName = "", bool setCursorHere = false, bool multiLine = false, const std::string& userData = "", bool doUpdateSize = true);
	void addEntry(const std::string& name, bool add_arrow = false, const std::function<void()>& func = nullptr, const std::string& iconName = "", bool setCursorHere = false, bool onButtonRelease = false, const std::string& userData = "", bool doUpdateSize = true);
	void addGroup(const std::string& label, bool forceVisible = false, bool doUpdateSize = true) { mList->addGroup(label, forceVisible); if (doUpdateSize) updateSize(); }
	void removeLastRowIfGroup(bool doUpdateSize = true) { mList->removeLastRowIfGroup(); if (doUpdateSize) updateSize(); }
	void removeEntry(const std::string& userData, bool doUpdateSize = true) { mList->remove(userData); if (doUpdateSize) updateSize(); }

	void addButton(const std::string& label, const std::string& helpText, const std::function<void()>& callback);

	void setTitle(const std::string& title, const std::shared_ptr<Font>& font = nullptr);
	void setSubTitle(const std::string& text);
	void setTitleImage(std::shared_ptr<ImageComponent> titleImage, bool replaceTitle = false);

	inline void setCursorToList() { mGrid.setCursorTo(mList); }
	inline void setCursorToButtons() { if (mButtonGrid) mGrid.setCursorTo(mButtonGrid); }
	inline bool isCursorToButtons() { return mButtonGrid && mGrid.isCursorTo(mButtonGrid); }

	inline int getCursorIndex() const { return mList->getCursorIndex(); }

	inline int size() const { return mList->size(); }
	std::string getSelected() { return mList->getSelected(); }

	virtual std::vector<HelpPrompt> getHelpPrompts() override;

	void onShow() override;

	float getHeaderGridHeight() const;
	float getTitleHeight() const;
	float getButtonGridHeight() const;

	void setMaxHeight(float maxHeight) 
	{ 
		if (mMaxHeight == maxHeight)
			return;

		mMaxHeight = maxHeight; 
		updateSize();
	}

	virtual void updateSize();
	void clearButtons();

	std::shared_ptr<ComponentList> getList() { return mList; };
	
	static void addMenuIcon(Window* window, ComponentListRow& row, const std::string& iconName);

	void addTab(const std::string label, const std::string value = "", bool setCursorHere = false);
	inline void setOnTabIndexChanged(const std::function<void()>& callback) { mOnTabIndexChanged = callback; };
	int getTabIndex() { return mTabIndex; }

	bool input(InputConfig* config, Input input) override;

protected:
	virtual void OnTabChanged(int newIndex) { if (mOnTabIndexChanged != nullptr) mOnTabIndexChanged();  };

private:
	void updateGrid();

	NinePatchComponent mBackground;
	ComponentGrid mGrid;

	std::shared_ptr<ComponentGrid> mHeaderGrid;
	std::shared_ptr<TextComponent> mTitle;
	std::shared_ptr<TextComponent> mSubtitle;
	std::shared_ptr<ImageComponent> mTitleImage;
	std::shared_ptr<ComponentList> mList;
	std::shared_ptr<ComponentGrid> mButtonGrid;
	std::shared_ptr<ComponentTab>	mTabs;
	int mTabIndex;
	std::vector< std::shared_ptr<ButtonComponent> > mButtons;

	float mMaxHeight;
	std::function<void()> mOnTabIndexChanged;
};

#endif // ES_CORE_COMPONENTS_MENU_COMPONENT_H
