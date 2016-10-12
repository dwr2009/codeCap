"use strict"; 
(function() {

    var name = "Work Time";

    var WorkTimes = function() {
        var parent = new window.MainMenuScreen(name);
        var self = Object.create(parent);
        self.$root.attr('id', 'worktime');

        var curLabelDescription = {
            name: 'worktime_currenttimename :: currenttimename',
            value: ''
        };
        var $worktime = $('<div>').appendTo(self.$root);
        var $worktime_first_lable = $('<div>').appendTo($worktime);
        var worktime_currenttime = new window.LabelView(curLabelDescription);
        worktime_currenttime.$root.appendTo($worktime_first_lable);
        worktime_currenttime.show();

        var newLabelDescription = {
            name: 'worktime_newtimename :: newtimename',
            value: ''
        };
        var $worktime_second_lable = $('<div>').addClass('worktime_second_lable').appendTo($worktime);
        var worktime_newtime = new window.LabelView(newLabelDescription);
        worktime_newtime.$root.appendTo($worktime_second_lable);
        worktime_newtime.show();

        var inputDescription = {
            name: 'worktime_passwordname :: passwordname',
            size: 6,
            type: 'password',
        };
        var $worktime_third_lable = $('<div>').addClass('worktime_lable').appendTo($worktime);
        var input = new window.InputView(inputDescription);
        input.$root.appendTo($worktime_third_lable);
        input.show();

        var $worktime_forth_lable = $('<div>').addClass('messagelable').appendTo($worktime);
        var $worktime_message = $('<div>').addClass('message').appendTo($worktime_forth_lable);

        var $worktime_fifth_lable = $('<div>').appendTo($worktime);
        var $worktime_left_area = $('<div>').addClass('column_left').appendTo($worktime_fifth_lable);
        var $worktime_right_area = $('<div>').addClass('column_right').appendTo($worktime_fifth_lable);
        var $worktime_confirm_panel = $('<div>').addClass('panel').append($TR('pincode_btn_confirm :: confirm')).appendTo($worktime_left_area);
        var $worktime_cancel_panel = $('<div>').addClass('panel').append($TR('IPPV_btn_cancel :: cancel')).appendTo($worktime_right_area);

        var $worktime_last_lable = $('<div>').addClass('remindlable').appendTo($worktime);
        var $redwarning = $('<div>').addClass('redwarning').appendTo($worktime_last_lable);
        var $redwarningtext = $('<div>').addClass('redwarningtext').append($TR('worktime_redwarning :: redwarning')).appendTo($worktime_last_lable);
        var listData = [{
            name: 'newtimeInput',
            $root: $worktime_second_lable,
            callback: null,
        },
        {
            name: 'passwordInput',
            $root: $worktime_third_lable,
            callback: _.bind(input.activate, input),
        },
        {
            name: 'btnConfirm',
            $root: $worktime_confirm_panel,
            callback: null,
        },
        {
            name: 'btnCancel',
            $root: $worktime_cancel_panel,
            callback: null,
        }];

        var focusList = new window.FocusList(listData);

        var workTimeList = function() {
            var wt = Platform.ca.GetWorkTime();
            worktime_currenttime.setText(wt.retVal?"":window.Format.formatWorkTime(wt));
        };

        var updateNewWorkTime = function() {
            var startTime = "";
            var endTime = "";

            for (var i = 0; i < 12; i++) {
                if (i < 6) {
                    if (i < timeInputNum) {
                        startTime += timeInput[i];
                    } else {
                        startTime += "-";
                    }
                    if (i == 1 || i == 3) {
                        startTime += ":";
                    }
                }
                else {
                    if (i < timeInputNum) {
                        endTime += timeInput[i];
                    } else {
                        endTime += "-";
                    }
                    if (i == 7 || i == 9) {
                        endTime += ":";
                    }
                }
            }
            worktime_newtime.setText(startTime + " - " + endTime);
        };

        var selection = 0;
        var flag = 0;

        var timeInputNum = 0;
        var timeInput = new Array();

				var handleSmcEvent = function(param){
			 			if(param.smcStatus == 0){
			 					self.onKeyBACK();
			 			}
			 	};
        var updateScreen = function() {
            flag = 0;
            focusList.select(0);
            $worktime_message.text("");
            timeInputNum = 0;
            for (var i = 0; i < 12; i++) {
                timeInput[i] = 0;
            }
            updateNewWorkTime();
            input.clear();
            input.activate(0);
            workTimeList();
            window.Platform.ca.setNotifySmcStatusCallback(handleSmcEvent);
        }

        var setNewWorktime = function() {
            flag = 1;
            var wt = {
                startHour: timeInput[0] * 10 + timeInput[1],
                startMin: timeInput[2] * 10 + timeInput[3],
                startSec: timeInput[4] * 10 + timeInput[5],
                endHour: timeInput[6] * 10 + timeInput[7],
                endMin: timeInput[8] * 10 + timeInput[9],
                endSec: timeInput[10] * 10 + timeInput[11],
                pinCode: input.getText()
            };
            var ret = Platform.ca.SetWorkTime(wt);
            $("#worktime   .message").css('color', ret.retVal ? 'red': 'green');
            switch (ret.retVal) {
            case 0:
                $worktime_message.text(TR('worktime :: Working Hours changed successfully'));
                break;
            case 3:
                $worktime_message.text(TR('IPPV_Unidentified Card!'));
                break;
            case 4:
                $worktime_message.text(TR('Pincode :: Invalid PIN'));
                break;
            case 0x0B:
                $worktime_message.text(TR('worktime :: Invalid working hours'));
                break;
            default:
                $worktime_message.text(TR('worktime :: Working Hours changed failed'));
                break;
            }
        }
        self.onKey = function() {
            return true;
        };

        self.onShow = function() {
            updateScreen();
        };
        self.onKeyOK = function() {
            if (focusList.getSelectedName() == 'btnConfirm') {
                if (timeInputNum < 12) {
                    $worktime_message.text(TR('worktime :: Invalid working hours'));
                } else {
                    setNewWorktime();
                    workTimeList();
                }
            } else if (focusList.getSelectedName() == 'btnCancel') {
            		window.Platform.ca.setNotifySmcStatusCallback(function(st){});
                window.AppManager.back();
            }
        }
        self.onKeyNUMBER = function(num) {
            var focusListname = focusList.getSelectedName();
            switch (focusListname) {
            case 'newtimeInput':
                {
                    if (timeInputNum < 12) {
                        var val = ((timeInputNum % 2) == 1) ? (timeInput[timeInputNum - 1] * 10 + num) : (num * 10 + timeInput[timeInputNum + 1]);
                        if ((timeInputNum % 6) < 2) {
                            if (val < 24) {
                                timeInput[timeInputNum] = num;
                                timeInputNum += 1;
                                updateNewWorkTime();
                            }
                        } else if (val < 60) {
                            timeInput[timeInputNum] = num;
                            timeInputNum += 1;
                            updateNewWorkTime();
                        }
                    }
                    break;
                }
            case 'passwordInput':
                {
                    input.appendChar(num.toString());
                    break;
                }
            }
        };
        self.onKeyBACK = 
        self.onKeyLEFT = function() {
        		window.Platform.ca.setNotifySmcStatusCallback(function(st){});
            window.AppManager.back();
        }
        self.onKeyRIGHT = function() {
            if (selection == worktime_input.length) {
                selection = 0;
            }
            setSelectedFocus(selection, "down");
            selection++;
        };

        self.onKeyUP = function() {
            $worktime_message.text("");
            if (flag == 1) {
                input.clear();
                timeInputNum = 0;
                for (var i = 0; i < 12; i++) {
                    timeInput[i] = 0;
                }
                updateNewWorkTime();
                flag = 0;
            }
            focusList.selectPrev();
        }
        self.onKeyDOWN = function() {
            $worktime_message.text("");
            if (flag == 1) {
                input.clear();
                timeInputNum = 0;
                for (var i = 0; i < 12; i++) {
                    timeInput[i] = 0;
                }
                updateNewWorkTime();
                flag = 0;
            }
            focusList.selectNext();
        };

        self.onKeyRED = function() {
            var focusListname = focusList.getSelectedName();
            switch (focusListname) {
            case 'newtimeInput':
                {
                    if (timeInputNum > 0) {
                        timeInputNum -= 1;
                        timeInput[timeInputNum] = 0;
                        updateNewWorkTime();
                    }
                    break;
                }
            case 'passwordInput':
                {
                    input.removeChar();
                    break;
                }
            }
        };
        return self;
    };

    window.AppManager.addScene(name, WorkTimes);
})();