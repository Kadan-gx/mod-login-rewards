# ![logo](https://raw.githubusercontent.com/azerothcore/azerothcore.github.io/master/images/logo-github.png) AzerothCore

## Login Rewards




This is a module for [AzerothCore](http://www.azerothcore.org) that adds login daily rewards.



Current features:

This module allows you to setup a simple daily login reward track. 
Every day the players will receive their reward through the mailbox. 
After the last day is reached, the track resets. 

You can setup the number of days of the track in the conf file. 
Make sure to edit the database script login_rewards_data_populate.sql as you see fit.

You can open a issue if you find any bugs or want some additional features! 


AzerothCore v1.0.1+

## How to install

###1) Simply place the module under the `modules` folder of your AzerothCore source folder.

You can do clone it via git under the azerothcore/modules directory:

`cd path/to/azerothcore/modules`

`git clone https://github.com/Kadan-gx/mod-login-rewards.git`

or you can manually download the module

assign the script to an item such as hearthstone using the script name 'premium_account'

###2) Re-run cmake and launch a clean build of AzerothCore

**That's it.**
.
