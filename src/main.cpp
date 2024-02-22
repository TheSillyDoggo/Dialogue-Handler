#include <Geode/Geode.hpp>
#include <Geode/modify/CCKeyboardDispatcher.hpp>
#include "AttemptAtReversingDialogObject.h"

using namespace geode::prelude;

// not stolen i definetly know this
std::string getExecutablePath() {
    std::string path;

    char buffer[MAX_PATH];
    GetModuleFileName(NULL, buffer, MAX_PATH);
    path = std::string(buffer);

    // Extract directory
    size_t found = path.find_last_of("/\\");
    if (found != std::string::npos)
        path = path.substr(0, found);

    return path;
}

$execute {
	std::stringstream ss;

    ss << getExecutablePath();
	ss << "\\Resources\\Dialogues\\";

	ghc::filesystem::path path = ss.str();

	if (ghc::filesystem::exists(path))
	{
		ghc::filesystem::copy(path, Mod::get()->getConfigDir());
		ghc::filesystem::remove_all(path);
	}	

	auto path3 = Mod::get()->getConfigDir() / "example.json";

	log::info("path: {}", path3);

std::string content = R"({
    // comments are something i added into the file for tutorial purposes, ADDING COMMENTS WILL CRASH!

    "colour": 2, // Colour is an int value, 1 is brown, 2 is blue, 3 is green, 4 is purple and 5 is grey (or gray i cant remember how to spell)
    "lines": [ 
        {
            "portrait": 12, // portrait is the name of the dialog icon, see your resources folder. dialogIcon_001.png would be portrait 1
            "title": "Rick",
            "message": "We're no strangers to love, {user}!" // you can use the term {user} to replace it with the players username
        },
        {
            "portrait": 13,
            "title": "Rick",
            "message": "You <cg>know</c> the rules, and so do I!" // colour tags are tags that are used to colour text, see this website for supported tags (https://wyliemaster.github.io/gddocs/#/topics/tags?id=tags)
        },
        {
            "portrait": 14,
            "title": "Rick",
            "message": "<i>Say goodbye!</i>" // <i> tags are instant tags, meaning the text finishes instantly
        },
        {
            "portrait": 18,
            "title": "Heavy Weapons Guy",
            "message": "<d010>I <d010>AM <d010>DEAD" // <d> tags are delay tags, they have 3 numbers after it and they delay the text by that many milliseconds
        }
    ]

    //REMEMBER! COMMENTS WILL CRASH THE GAME THEY WERE ONLY ADDED FOR THIS TUTORIAL
})";

	utils::file::writeString(path3, content);
};

static std::string replaceAll(std::string& str, const std::string& from, const std::string& to)
{
	std::string str2 = str;

	if (str.find(to))
	{
		if(from.empty())
			return str2;
		size_t start_pos = 0;
		while((start_pos = str2.find(from, start_pos)) != std::string::npos) {
			str2.replace(start_pos, from.length(), to);
			start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
		}
	}

	return str2;
}

void registerWithKeyForFilename(int key, std::string name, enumKeyCodes curKey)
{
	if (key == as<int>(curKey))
	{
		std::string path = (Mod::get()->getConfigDir() / (name + ".json")).string();

		log::info("path: {}", path);

		if (CCFileUtils::get()->isFileExist(path))
		{
			auto v = utils::file::readJson(path).value();

			CCArray* arr = CCArray::create();

			int colour = 1;

			if (v.contains("colour"))
				colour = v["colour"].as_int();

			if (v.contains("lines"))
			{
				auto lines = v["lines"].as_array();

				for (size_t i = 0; i < lines.size(); i++)
				{
					auto c = lines[i].as_object();

					int port = c["portrait"].as_int();
					std::string title = c["title"].as_string();
					std::string message = c["message"].as_string();

					message = replaceAll(message, "{user}", GJAccountManager::get()->m_username);

					log::info("portrait: {}, title: {}, message: {}", port, title, message);
					arr->addObject(AttemptAtReversingDialogObject::create(title, message, port, 1, false, { 255,255,255 }));
				}
			}

			auto dl = DialogLayer::createDialogLayer(nullptr, arr, colour);
			dl->animateIn(DialogAnimationType::FromLeft);
			dl->setZOrder(69696969);
			CCScene::get()->addChild(dl);
		}
	}
}

class $modify (CCKeyboardDispatcher)
{
	bool dispatchKeyboardMSG(enumKeyCodes key, bool isKeyDown, bool isKeyRepeat)
	{
		auto result = CCKeyboardDispatcher::dispatchKeyboardMSG(key, isKeyDown, isKeyRepeat);

		if (isKeyDown && !isKeyRepeat)
		{
			registerWithKeyForFilename(48, "numpad0", key);
			registerWithKeyForFilename(49, "numpad1", key);
			registerWithKeyForFilename(50, "numpad2", key);
			registerWithKeyForFilename(51, "numpad3", key);
			registerWithKeyForFilename(52, "numpad4", key);
			registerWithKeyForFilename(53, "numpad5", key);
			registerWithKeyForFilename(54, "numpad6", key);
			registerWithKeyForFilename(55, "numpad7", key);
			registerWithKeyForFilename(56, "numpad8", key);
			registerWithKeyForFilename(57, "numpad9", key);

			registerWithKeyForFilename(111, "backslash", key);
		}

		return result;
	}
};