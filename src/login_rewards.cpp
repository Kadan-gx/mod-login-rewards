/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license: https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
 */

#include "ScriptMgr.h"
#include "Player.h"
#include "Config.h"
#include "Chat.h"


struct LoginRewardsItems
{
    int32_t itemId = 0;
    uint8_t count = 0;
};
std::map <uint8_t, LoginRewardsItems> rewardsData;

class WorldScript_LoginRewards : public WorldScript
{
public:
    WorldScript_LoginRewards() : WorldScript("WorldScript_Bounty"){}


    void LoadData()
    {
        if (!sConfigMgr->GetOption<bool>("LoginRewards", true))
        {
            return;
        }

        QueryResult result = WorldDatabase.Query("SELECT day,item,count FROM login_rewards_data");

        if (!result)
        {
            return;
        }


        do
        {
            Field* fields = result->Fetch();

            auto day = fields[0].Get<uint8_t>();
            auto itemId = fields[1].Get<int32_t>();
            auto count = fields[2].Get<uint8_t>();


            rewardsData.insert({ day, { itemId,count } });
        } while (result->NextRow());
    }

    void OnStartup() override
    {
        LoadData();
    }
    void OnAfterConfigLoad(bool reload) override
    {
        if (reload)
            LoadData();
    }

};
// Add player scripts
class PlayerScript_LoginRewards : public PlayerScript
{
public:

    PlayerScript_LoginRewards() : PlayerScript("LoginRewards"){}

    void OnLogin(Player* player) override
    {
        if (!sConfigMgr->GetOption<bool>("LoginRewards", true))
        {
            return;
        }
        auto maxRewards = sConfigMgr->GetOption<int32>("MaxRewards", 30);
        QueryResult result = CharacterDatabase.Query("SELECT lastClaimed,reward FROM login_rewards WHERE guid={}", player->GetGUID().GetCounter());

        int rewardId = 0;
        if (!result) // new player
        {
            CharacterDatabase.Execute("INSERT INTO login_rewards VALUES('{}','{}','{}')", player->GetGUID().GetCounter(), (uint32_t)std::time(nullptr), 0);

        }
        else
        {
            Field* fields = result->Fetch();


            auto lastClaimed = time_t(fields[0].Get<uint32>());
            rewardId = fields[1].Get<uint8>();

            auto currentTime = Acore::Time::TimeBreakdown(std::time(nullptr));
            auto lastClaimedBkd = Acore::Time::TimeBreakdown(lastClaimed);

            if (lastClaimedBkd.tm_mday == currentTime.tm_mday
                && lastClaimedBkd.tm_mon == currentTime.tm_mon
                && lastClaimedBkd.tm_year == currentTime.tm_year)


            {
                return;
            }
            else
            {

                // send reward to player:
                if (rewardId + 1 >= maxRewards) // reset the track
                {
                    rewardId = 0;
                }

                CharacterDatabase.Execute("INSERT INTO login_rewards VALUES('{}','{}','{}')", player->GetGUID().GetCounter(), (uint32_t)std::time(nullptr), rewardId);



            }
        }

        if (auto it = rewardsData.find(rewardId); // no data found
            it == rewardsData.end())
        {
            return;
        }
        else
        {
            MailDraft draft(MAIL_NORMAL);



            CharacterDatabaseTransaction trans = CharacterDatabase.BeginTransaction();

            Item* item = it->second.itemId ? Item::CreateItem(it->second.itemId, it->second.count, player) : nullptr;
            if (item)
            {
                // save new item before send
                item->SaveToDB(trans);                               // save for prevent lost at next mail load, if send fail then item will deleted

                // item
                draft.AddItem(item);
                ChatHandler(player->GetSession()).PSendSysMessage("Your daily login reward has been sent to your mailbox!");
            }

            draft.SendMailTo(trans, player, MailSender(MAIL_NORMAL, 0));
            CharacterDatabase.CommitTransaction(trans);
        }
    }
};

// Add all scripts in one
void AddMyLoginRewards()
{
    new PlayerScript_LoginRewards();
    new WorldScript_LoginRewards();
}
