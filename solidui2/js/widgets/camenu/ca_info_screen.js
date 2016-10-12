"use strict"; (function() {

    var name = "CA Information";

    var Disentitlelist = new window.Class(
    //parent
    window.ListViewDelegate,

    //constructor
    function(disentitleinfo) {
        ListViewDelegate.call(this, name);
        this.$root.addClass('lable');
        var $Disentitle_confirmcode = $('<div>').addClass('column_half').text(disentitleinfo.ConfirmCode).appendTo(this.$root);
        var $Disentitle_status = $('<div>').addClass('column_half').text(disentitleinfo.Status).appendTo(this.$root);
    },
    //prototype
    {});
    var Entitlementlist = new window.Class(
    //parent
    window.ListViewDelegate,

    //constructor
    function(entitlementinfo) {
        ListViewDelegate.call(this, name);
        this.$root.addClass('lable');
        var $Entitlement_productid = $('<div>').addClass('column_three').text(entitlementinfo.ProductId).appendTo(this.$root);
        var $Entitlement_recordingenable = $('<div>').addClass('column_three').text(entitlementinfo.RecordingEnable).appendTo(this.$root);
        if (entitlementinfo.EndDate instanceof Date) {
            var date_string = window.Format.formatDate(entitlementinfo.EndDate);
            var $Entitlement_endtime = $('<div>').addClass('column_three').text(date_string).appendTo(this.$root);
        } else {
            var $Entitlement_endtime = $('<div>').addClass('column_three').appendTo(this.$root);
        }
    },
    //prototype
    {});
    var Slotlist = new window.Class(
    //parent
    window.ListViewDelegate,

    //constructor
    function(slotinfo) {
        ListViewDelegate.call(this, name);
        this.$root.addClass('lable');
        var $Slot_productid = $('<div>').addClass('column_three').text(slotinfo.SlotId).appendTo(this.$root);
        var $Slot_spendcount = $('<div>').addClass('column_three').text(slotinfo.SpendCount).appendTo(this.$root);
        var $Slot_creditline = $('<div>').addClass('column_three').text(slotinfo.creditline).appendTo(this.$root);

    },
    //prototype
    {});
    var AClist = new window.Class(
    //parent
    window.ListViewDelegate,

    //constructor
    function(acinfo) {
        ListViewDelegate.call(this, name);
        this.$root.addClass('lable');
        var $AClist_left = $('<div>').addClass('column_half').text(acinfo.text).appendTo(this.$root);
        var $AClist_right = $('<div>').addClass('column_half').text(acinfo.value).appendTo(this.$root);
    },
    //prototype
    {}

    );
    var Ippvlist = new window.Class(

    // parent
    window.ListViewDelegate,

    // constructor
    function(ippvinfo) {
        ListViewDelegate.call(this, name);
        this.$root.addClass('lable');

        var $IPPV_sloteid = $('<div>').addClass('column_six').text(ippvinfo.SlotId).appendTo(this.$root);
        var $IPPV_programid = $('<div>').addClass('column_six').text(ippvinfo.ProgramId).appendTo(this.$root);
        var $IPPV_productstatus = $('<div>').addClass('column_six').text(ippvinfo.ProductStatus).appendTo(this.$root);
        var $IPPV_recordingenable = $('<div>').addClass('column_six').text(ippvinfo.RecordingEnable).appendTo(this.$root);
        var $IPPV_price = $('<div>').addClass('column_six').text(ippvinfo.Price).appendTo(this.$root);
        if (ippvinfo.endTime instanceof Date) {
            var date_string = window.Format.formatDate(ippvinfo.endTime);
            var $IPPV_endtime = $('<div>').addClass('column_six').text(date_string).appendTo(this.$root);
        }
    },

    // prototype
    {});
    var PCcardlist = new window.Class(

    // parent
    window.ListViewDelegate,

    // constructor
    function(pcCard) {
        ListViewDelegate.call(this, name);
        this.$root.addClass('lable');

        var $ParentChildCard_operatorid = $('<div>').addClass('column_fifteen').text(pcCard.ischild).appendTo(this.$root);
        var $ParentChildCard_cardtype = $('<div>').addClass('column_fifteen').text(pcCard.iscanfeed).appendTo(this.$root);
        var $ParentChildCard_parentcardid = $('<div>').addClass('column_fifteen').text(pcCard.delaytime).appendTo(this.$root);
        if (pcCard.lastfeedtime instanceof Date) {
            var date_string = window.Format.formatDate(pcCard.lastfeedtime) + " " + window.Format.formatTime(pcCard.lastfeedtime);
            var $ParentChildCard_lastfeedtime = $('<div>').addClass('column_four').text(date_string).appendTo(this.$root);
        }
	else {
            var $ParentChildCard_lastfeedtime = $('<div>').addClass('column_four').appendTo(this.$root);
	}
        var $ParentChildCard_feedinterval = $('<div>').addClass('column_four').text(pcCard.ParentcardSN).appendTo(this.$root);
    },

    // prototype
    {});

    var Delegate = new window.Class(

    // parent
    window.ListViewDelegate,

    // constructor
    function(description) {
        ListViewDelegate.call(this, description.name);

        var select_view = new window.SelectView(description, description.action);
        select_view.$root.appendTo(this.$root);
        select_view.show();

        this.select_view = select_view;
    },

    // prototype
    {

});

    var CAinformations = function() {

        var parent = new window.MainMenuScreen(name);
        var self = Object.create(parent);
        self.$root.attr('id', 'CAinformation');

        var showType = function(typename) {
            switch (typename) {
            case 'Entitlement':
                {
                    $("#CAinformation .Entitlement").css('display', 'inline');
                    $("#CAinformation .Slot").css('display', 'none');
                    $("#CAinformation .ACList").css('display', 'none');
                    $("#CAinformation .IPPV").css('display', 'none');
                    $("#CAinformation .Disentitle").css('display', 'none');
                    $("#CAinformation .ParentChildCard").css('display', 'none');
                    entitlementlist.show();
                    break;
                }
            case 'Slot':
                {
                    $("#CAinformation .Entitlement").css('display', 'none');
                    $("#CAinformation .Slot").css('display', 'inline');
                    $("#CAinformation .ACList").css('display', 'none');
                    $("#CAinformation .IPPV").css('display', 'none');
                    $("#CAinformation .ParentChildCard").css('display', 'none');
                    $("#CAinformation .Disentitle").css('display', 'none');
                    slotlist.show();
                    break;
                }
            case 'ACList':
                {
                    $("#CAinformation .Entitlement").css('display', 'none');
                    $("#CAinformation .Slot").css('display', 'none');
                    $("#CAinformation .ACList").css('display', 'inline');
                    $("#CAinformation .IPPV").css('display', 'none');
                    $("#CAinformation .ParentChildCard").css('display', 'none');
                    $("#CAinformation .Disentitle").css('display', 'none');
                    aclist.show();
                    break;
                }
            case 'IPPV':
                {
                    $("#CAinformation .Entitlement").css('display', 'none');
                    $("#CAinformation .Slot").css('display', 'none');
                    $("#CAinformation .ACList").css('display', 'none');
                    $("#CAinformation .IPPV").css('display', 'inline');
                    $("#CAinformation .ParentChildCard").css('display', 'none');
                    $("#CAinformation .Disentitle").css('display', 'none');
                    ippvlist.show();
                    break;
                }
            case 'ParentChildCard':
                {
                    $("#CAinformation .Entitlement").css('display', 'none');
                    $("#CAinformation .Slot").css('display', 'none');
                    $("#CAinformation .ACList").css('display', 'none');
                    $("#CAinformation .IPPV").css('display', 'none');
                    $("#CAinformation .ParentChildCard").css('display', 'inline');
                    $("#CAinformation .Disentitle").css('display', 'none');
                    parentchildcardlist.show();
                    break;
                }
            case 'Disentitle':
                {
                    $("#CAinformation .Entitlement").css('display', 'none');
                    $("#CAinformation .Slot").css('display', 'none');
                    $("#CAinformation .ACList").css('display', 'none');
                    $("#CAinformation .IPPV").css('display', 'none');
                    $("#CAinformation .ParentChildCard").css('display', 'none');
                    $("#CAinformation .Disentitle").css('display', 'inline');
                    disentitlelist.show();
                    break;
                }
            case 'none':
                {
                    $("#CAinformation .Entitlement").css('display', 'none');
                    $("#CAinformation .Slot").css('display', 'none');
                    $("#CAinformation .ACList").css('display', 'none');
                    $("#CAinformation .IPPV").css('display', 'none');
                    $("#CAinformation .ParentChildCard").css('display', 'none');
                    $("#CAinformation .Disentitle").css('display', 'none');
                }
            }
        };
        var switchToOperator = function(item) {
            console.log("Selected :operator", item.name);
            setOperator(item.value);
            var typeName = cainfolist.getItemByIndex(1).select_view.getSelected().value;
            setType(typeName);
            var currentType = getType();
            if (currentType == 'Entitlement') {
                getCAInfoDataEntitles();
            }
            if (currentType == 'Slot') {
                getCAInfoDataSlot();
            }
            if (currentType == 'ACList') {
                getCAInfoDataACList();
            }
            if (currentType == 'IPPV') {
                getCAInfoDataIppv();
            }
            if (currentType == 'ParentChildCard') {
                getCAInfoDataPCCard();
            }
            if (currentType == 'Disentitle') {
                getCAInfoDataDisentitle();
            }
            showType(currentType);
        };

        var switchToType = function(item) {
            console.log("Selected :type", item.name);
            var typeName = item.value;
            setType(typeName);

            if (typeName == 'Entitlement') {
                getCAInfoDataEntitles();
            }
            if (typeName == 'Slot') {
                getCAInfoDataSlot();
            }
            if (typeName == 'ACList') {
                getCAInfoDataACList();
            }
            if (typeName == 'IPPV') {
                getCAInfoDataIppv();
            }
            if (typeName == 'ParentChildCard') {
                getCAInfoDataPCCard();
            }
            if (typeName == 'Disentitle') {
                getCAInfoDataDisentitle();
            }
            showType(typeName);
        }

        var description = [{
            name: "CAinformation screen :: operator",
            items: [],
            action: switchToOperator,
        },
        {
            name: "CAinformation screen :: type",
            items: [{
                name: 'CAinformation screen :: Entitlement',
                value: 'Entitlement',
            },
            {
                name: 'CAinformation screen :: Slot',
                value: 'Slot',
            },
            {
                name: 'CAinformation screen :: IPPV',
                value: 'IPPV',
            },
            {
                name: 'CAinformation screen :: ParentChildCard',
                value: 'ParentChildCard',
            },
            //{
                //name: 'CAinformation screen :: Disentitle',
                //value: 'Disentitle',
            //},
            {
                name: 'CAinformation screen :: ACList',
                value: 'ACList',
            }
            ],
            action: switchToType,
        },
        ];

        var cainfolist = new window.ListView(Delegate, description);
        cainfolist.$root.appendTo(self.$root);
        cainfolist.show();

        var select_view = null;

        var $Entitlement = $('<div>').addClass('Entitlement').appendTo(self.$root);
        var $Entitlement_third_lable = $('<div>').addClass('caption').appendTo($Entitlement);
        var $Entitlement_productidname = $('<div>').addClass('column_three').append($TR('Entitlement_productidname :: productidname')).appendTo($Entitlement_third_lable);
        var $Entitlement_recordingenablename = $('<div>').addClass('column_three').append($TR('Entitlement_recordingenablename :: recordingenablename')).appendTo($Entitlement_third_lable);
        var $Entitlement_endtimename = $('<div>').addClass('column_three').append($TR('Entitlement_endtimename :: endtimename')).appendTo($Entitlement_third_lable);

        var $Entitlement_forth_lable = $('<div>').appendTo($Entitlement);
				var entitlementlist = new window.pageTurningListView( Entitlementlist ) ;
        entitlementlist.$root.appendTo($Entitlement_forth_lable);

        var $Entitlement_errorMsg_lable = $('<div>').addClass('messagelable').appendTo($Entitlement_forth_lable);
        var $Entitlement_errorMsg_message = $('<div>').addClass('message').appendTo($Entitlement_errorMsg_lable);

        var $Slot = $('<div>').addClass('Slot').appendTo(self.$root);
        var $Slot_third_lable = $('<div>').addClass('caption').appendTo($Slot);
        var $Slot_productidname = $('<div>').addClass('column_three').append($TR('Slot_productidname :: productidname')).appendTo($Slot_third_lable);
        var $Slot_spendcountname = $('<div>').addClass('column_three').append($TR('Slot_spendcountname :: spendcountname')).appendTo($Slot_third_lable);
        var $Slot_creditlinename = $('<div>').addClass('column_three').append($TR('Slot_creditlinename :: creditlinename')).appendTo($Slot_third_lable);

        var $Slot_forth_lable = $('<div>').appendTo($Slot);
				var slotlist = new window.pageTurningListView( Slotlist ) ;
        slotlist.$root.appendTo($Slot_forth_lable);
        
        var $Slot_errorMsg_lable = $('<div>').addClass('messagelable').appendTo($Slot_forth_lable);
        var $Slot_errorMsg_message = $('<div>').addClass('message').appendTo($Slot_errorMsg_lable);

        var $ACList = $('<div>').addClass('ACList').appendTo(self.$root);
        var $ACList_third_lable = $('<div>').addClass('caption').append($TR('ACList_caption :: caption')).appendTo($ACList);

        var $ACList_forth_lable = $('<div>').appendTo($ACList);
		 		var aclist = new window.pageTurningListView( AClist ) ;
	aclist.setDefaultItemsNum(9);
	aclist.hidePageCount();
	aclist.hideHelpInfo();
        aclist.$root.appendTo($ACList_forth_lable);

        var $ACList_errorMsg_lable = $('<div>').addClass('messagelable').appendTo($ACList);
        var $ACList_errorMsg_message = $('<div>').addClass('message').appendTo($ACList_errorMsg_lable);

        var $IPPV = $('<div>').addClass('IPPV').appendTo(self.$root);
        var $IPPV_third_lable = $('<div>').addClass('caption').appendTo($IPPV);
        var $IPPV_slotidname = $('<div>').addClass('column_six').append($TR('IPPV_slotname :: slotname')).appendTo($IPPV_third_lable);
        var $IPPV_programidname = $('<div>').addClass('column_six').append($TR('IPPV_programidname :: programidname')).appendTo($IPPV_third_lable);
        var $IPPV_productstatusname = $('<div>').addClass('column_six').append($TR('IPPV_productstatusname :: productstatusname')).appendTo($IPPV_third_lable);
        var $IPPV_recordingenablename = $('<div>').addClass('column_six').append($TR('IPPV_recordingenablename :: recordingenablename')).appendTo($IPPV_third_lable);
        var $IPPV_pricename = $('<div>').addClass('column_six').append($TR('IPPV_price :: price')).appendTo($IPPV_third_lable);
        var $IPPV_endtimename = $('<div>').addClass('column_six').append($TR('IPPV_endtimename :: endtimename')).appendTo($IPPV_third_lable);
        
        var $IPPV_forth_lable = $('<div>').appendTo($IPPV);
			  var ippvlist = new window.pageTurningListView( Ippvlist ) ;
        ippvlist.$root.appendTo($IPPV_forth_lable);
        
        //var $IPPV_last_lable = $('<div>').addClass('totalcountlable').appendTo($IPPV);
        //var $IPPV_totalcountname =$('<div>').addClass('totalcount').appendTo($IPPV_last_lable);
                
				var $IPPV_errorMsg_lable = $('<div>').addClass('messagelable').appendTo($IPPV_forth_lable);
        var $IPPV_errorMsg_message = $('<div>').addClass('message').appendTo($IPPV_errorMsg_lable);

        var $ParentChildCard = $('<div>').addClass('ParentChildCard').appendTo(self.$root);
        var $ParentChildCard_third_lable = $('<div>').addClass('caption').appendTo($ParentChildCard);
        var $ParentChildCard_operatoridname = $('<div>').addClass('column_fifteen').append($TR('parentchildcard_operatoridname :: operatoridname')).appendTo($ParentChildCard_third_lable);
        var $ParentChildCard_cardtypename = $('<div>').addClass('column_fifteen ').append($TR('parentchildcard_cardtypename :: cardtypename')).appendTo($ParentChildCard_third_lable);
        var $ParentChildCard_parentcardidname = $('<div>').addClass('column_fifteen ').append($TR('parentchildcard_parentcardidname :: parentcardidname')).appendTo($ParentChildCard_third_lable);
        var $ParentChildCard_feedintervalname = $('<div>').addClass('column_four ').append($TR('parentchildcard_feedintervalname :: feedintervalname')).appendTo($ParentChildCard_third_lable);
        var $ParentChildCard_lastfeedtimename = $('<div>').addClass('column_four ').append($TR('parentchildcard_lastfeedtimename :: lastfeedtimename')).appendTo($ParentChildCard_third_lable);

        var $ParentChildCard_forth_lable = $('<div>').appendTo($ParentChildCard);
			  var parentchildcardlist = new window.pageTurningListView( PCcardlist ) ;
        parentchildcardlist.hidePageCount();
        parentchildcardlist.hideHelpInfo();
        parentchildcardlist.$root.appendTo($ParentChildCard_forth_lable);

        var $ParentChildCard_last_lable = $('<div>').addClass('remindlable').appendTo($ParentChildCard);
        var $ParentChildCard_yellowwarning = $('<div>').addClass('yellowwarning').appendTo($ParentChildCard_last_lable);
        var $ParentChildCard_yellowwarningtext = $('<div>').addClass('yellowwarningtext').append($TR('ParentChildCard_feeding :: feeding')).appendTo($ParentChildCard_last_lable);

        var $ParentChildCard_errorMsg_lable = $('<div>').addClass('messagelable').appendTo($ParentChildCard_last_lable);
        var $ParentChildCard_errorMsg_message = $('<div>').addClass('message').appendTo($ParentChildCard_errorMsg_lable);

        var $Disentitle = $('<div>').addClass('Disentitle').appendTo(self.$root);
        var $Disentitle_third_lable = $('<div>').addClass('caption').appendTo($Disentitle);
        var $Disentitle_confirmcodename = $('<div>').addClass('column_half').append($TR('Disentitle_confirmcodename :: confirmcodename')).appendTo($Disentitle_third_lable);
        var $Disentitle_statusname = $('<div>').addClass('column_half').append($TR('Disentitle_statusname :: statusname')).appendTo($Disentitle_third_lable);

        var $Disentitle_forth_lable = $('<div>').appendTo($Disentitle);
			
			  var disentitlelist = new window.pageTurningListView( Disentitlelist ) ;
        disentitlelist.$root.appendTo($Disentitle_forth_lable);

        var $Disentitle_last_lable = $('<div>').addClass('remindlable').appendTo($Disentitle);
        var $Disentitle_redwarning = $('<div>').addClass('redwarning').appendTo($Disentitle_last_lable);
        var $Disentitle_redwarningtext = $('<div>').addClass('redwarningtext').append($TR('Disentitle_remove :: remove')).appendTo($Disentitle_last_lable);
        var $Disentitle_green_warning = $('<div>').addClass('greenwarning').appendTo($Disentitle_last_lable);
        var $Disentitle_green_warningtext = $('<div>').addClass('greenwarningtext').append($TR('Disentitle_removeall :: removeall')).appendTo($Disentitle_last_lable);

        var $Disentitle_errorMsg_lable = $('<div>').addClass('messagelable').appendTo($Disentitle_last_lable);
        var $Disentitle_errorMsg_message = $('<div>').addClass('message').appendTo($Disentitle_errorMsg_lable);

				var getOperatorsItems = function(){
						console.log("getOperatorsItems");
						var items = new Array();
		    		var op = Platform.ca.GetOperatorIds();		    		
						if(op.retVal == 0){				            
								for(var i = 0; i < op.operatorList.length; i++){
										if(op.operatorList[i] != 0){
												var a = op.operatorList[i];
												var item = {name:a,value:a,};
												items.push(item);
										}
								}	
						}
						return items ;
	   		}
	   		
	   		var operatorIndex = 0;
	   		var typeName = null;	   		
	   		var initOperator = function(){
		        var operatorsItems = getOperatorsItems();
		        if (operatorsItems.length) {
		        		cainfolist.getItemByIndex(0).select_view.updateItems(operatorsItems);        
		        		operatorIndex = cainfolist.getItemByIndex(0).select_view.getDefaultItemValue(operatorsItems);
		      	}	   			
	   		}
        var getOperator = function() {
            return operatorIndex;
        }
        var setOperator = function(index) {
            operatorIndex = index;
        }
        var getType = function() {
            return typeName;
        }
        var setType = function(name) {
            typeName = name;
        }
        
        var listData = [];
        var addSelectViewFocusList = function() {
            for (var i = 0; i < cainfolist.length; i++) {
                var json = {
                    $root: cainfolist.getItemByIndex(i).$root,
                    callback: null,
                };
                listData.push(json);
            }
        }

        var addFocusList = function() {
						var front = disentitlelist.getVisibleItemFront();
						var end = disentitlelist.getVisibleItemEnd();
						for(var j = front; j <= end; j++){	
		                var json = {
		                    $root: disentitlelist.getItem(j).$root,
		                    callback: null,
		                };
		
		                listData.push(json);
		            }
        }
        var removeFocusList = function() {
            for (var j = listData.length; j > 0; j--) {
                listData.pop();
            }
        }
        var focusList = new window.FocusList(listData);
        var getCAInfoDataEntitles = function() {
            var opid = {operatorID:getOperator()};
            var entitles = Platform.ca.GetServiceEntitles(opid);
            var q = new Array();
            if (entitles.retVal == 0) {
                for (var i = 0; i < entitles.entitleList.length; i++) {
                    var dt = new Date();
                    dt.setTime(Date.parse("2000/1/1") + entitles.entitleList[i].expiredDate * 24 * 3600 * 1000);
                    var p = {
                        ProductId: entitles.entitleList[i].productID,
                        RecordingEnable: entitles.entitleList[i].canTape ? TR('yes_no :: yes'): TR('yes_no :: no'),
                        EndDate: dt,
                    }
                    q.push(p);
                }
            }
            entitlementlist.updateItems(q);
            entitlementlist.getPageCountInit();
            removeFocusList();
            addSelectViewFocusList();
            focusList.updateItems(listData);
        }
        var getCAInfoDataSlot = function() {
            var opid = {operatorID:getOperator()};
            var slotid = Platform.ca.GetSlotIDs(opid);
            var q = new Array();          
            if (slotid.retVal == 0) {
                for (var i = 0; i < slotid.slotList.length; i++) {
                    var slot = {
                        operatorID: opid.operatorID,
                        slotID: slotid.slotList[i],
                    };
                    var slotinfo = window.Platform.ca.GetSlotInfo(slot);
                    var p = {
                        SlotId: slotid.slotList[i],
                        SpendCount: slotinfo.balance,
                        creditline: slotinfo.creditLimit,
                    };
                    q.push(p);
                }
            }
            slotlist.updateItems(q);
            slotlist.getPageCountInit();
            removeFocusList();
            addSelectViewFocusList();
            focusList.updateItems(listData);
        }
        var getCAInfoDataACList = function() {
            var opid = {operatorID:getOperator()};
            var ac = Platform.ca.GetACList(opid);
            if (ac.retVal == 0) {
                var aclistinfo = [{
                    text: TR('ACList_areacode :: areacode'),
                    value: ac.acList[0],
                },
                {
                    text: TR('ACList_bouquetid :: bouquetid'),
                    value: ac.acList[1],
                },
                {
                    text: TR('ACList_eigenvalue :: eigenvalue0'),
                    value: ac.acList[3],
                },
                {
                    text: TR('ACList_eigenvalue :: eigenvalue1'),
                    value: ac.acList[4],
                },
                {
                    text: TR('ACList_eigenvalue :: eigenvalue2'),
                    value: ac.acList[5],
                },
                {
                    text: TR('ACList_eigenvalue :: eigenvalue3'),
                    value: ac.acList[6],
                },
                {
                    text: TR('ACList_eigenvalue :: eigenvalue4'),
                    value: ac.acList[7],
                },
                {
                    text: TR('ACList_eigenvalue :: eigenvalue5'),
                    value: ac.acList[8],
                },
                {
                    text: TR('ACList_eigenvalue :: eigenvalue6'),
                    value: ac.acList[9],
                },
                ];
            }
            else {
            		var aclistinfo = [];
            }
            aclist.updateItems(aclistinfo);
            aclist.getPageCountInit();
            removeFocusList();
            addSelectViewFocusList();
            focusList.updateItems(listData);
        }
        var getCAInfoDataIppv = function() {
            var opid = {operatorID:getOperator()};
            var ippv = Platform.ca.GetIPPVProgram(opid);
            var q = new Array();
            if (ippv.retVal == 0) {
                for (var i = 0; i < ippv.ippvList.length; i++) {
                    var dt = new Date();
                    dt.setTime(Date.parse("2000/1/1") + ippv.ippvList[i].expiredDate * 24 * 3600 * 1000);
                    var p = {
                        SlotId: ippv.ippvList[i].slotID,
                        ProgramId: ippv.ippvList[i].productID,
                        ProductStatus: (ippv.ippvList[i].bookedFlag == 3) ? TR('IPPV_status :: viewed'): TR('IPPV_status :: booking'),
                        RecordingEnable: ippv.ippvList[i].canTape ? TR('yes_no :: yes'): TR('yes_no :: no'),
                        Price: ippv.ippvList[i].price,
                        endTime: dt,
                    }
                    q.push(p);
                }
                //$IPPV_totalcountname.text($TR('IPPV_totalcountname :: totalcountname')+q.length);
            }
            ippvlist.updateItems(q);
            ippvlist.getPageCountInit();
            removeFocusList();
            addSelectViewFocusList();
            focusList.updateItems(listData);
        }
        var getCAInfoDataPCCard = function() {
            var opid = {operatorID:getOperator()};
            var cs = Platform.ca.GetOperatorChildStatus(opid);
            var q = new Array();
            var cardinfo;
            if (cs.retVal == 0) {
                var dt = new Date();
                if (cs.isChild) {
                    var d = (cs.lastFeedTime >>> 16);
                    var h = (cs.lastFeedTime >>> 11) & 0x1F;
                    var m = (cs.lastFeedTime >>> 5) & 0x3F;
                    var s = cs.lastFeedTime & 0x1F;
                    dt.setTime(Date.parse("2000/1/1") + (d * 24 * 3600 + h * 3600 + m * 60 + s) * 1000);

                    cardinfo = {
                        ischild: cs.isChild ? TR('parentchildcard_cardtype :: childcard'): TR('parentchildcard_cardtype :: parentcard'),
                        iscanfeed: cs.isCanFeed ? TR('yes_no :: yes'): TR('yes_no :: no'),
                        delaytime: cs.delayTime + " " + TR('duration :: hour'),
                        ParentcardSN: cs.parentCardSN,
                        lastfeedtime: dt,
                    };
                } else {
                    cardinfo = {
                        ischild: cs.isChild ? TR('parentchildcard_cardtype :: childcard'): TR('parentchildcard_cardtype :: parentcard'),
                        iscanfeed: cs.isCanFeed ? TR('yes_no :: yes'): TR('yes_no :: no'),
                        delaytime: " ",
                        ParentcardSN: " ",
                        lastfeedtime: " ",
                    };
                } 
            }
            else {
                cardinfo = {
                        ischild: " ",
                        iscanfeed: " ",
                        delaytime: " ",
                        ParentcardSN: " ",
                        lastfeedtime: " ",
                    };
            }
            q.push(cardinfo);
            parentchildcardlist.updateItems(q);
            parentchildcardlist.getSelected().$root.removeClass("selected");
     				parentchildcardlist.getPageCountInit();
      			removeFocusList();
            addSelectViewFocusList();
            focusList.updateItems(listData);
        };
        var getCAInfoDataDisentitle = function() {
            var opid = {operatorID:getOperator()};
            var detitle = Platform.ca.GetDetitleChkNums(opid);
            var q = new Array();
            if (detitle.retVal == 0) {
                
                for (var i = 0; i < detitle.detitleList.length; i++) {
                    var p = {
                        ConfirmCode: detitle.detitleList[i],
                        Status: detitle.readFlag,
                    }
                    q.push(p);
                }								
            }
            disentitlelist.updateItems(q);								
						disentitlelist.getPageCountInit();
            removeFocusList();
            addSelectViewFocusList();
            addFocusList();
            focusList.updateItems(listData);
        }

				var handleSmcEvent = function(param){
			 			if(param.smcStatus == 0){
			 					self.onKeyBACK();
			 			}
			 	};
        var updateScreen = function() {
        		initOperator();
            getCAInfoDataEntitles();
            console.log("focusList.length = "+ focusList.length);
            focusList.select(0);
            cainfolist.show();

            for (var i = 0; i < cainfolist.length; i++) {
                var selectView = cainfolist.getItemByIndex(i).select_view;
                selectView.changeSelection(0);
            }
            typeName = null;
            showType('Entitlement');
						window.Platform.ca.setNotifySmcStatusCallback(handleSmcEvent);
        }
				var pCardDlg = window.AppManager.getWidget("confirm PopDlg");	
							
        var confirmChildCardInsert = function(){
    				var smc = Platform.ca.GetCardStatus();
    				if(smc.cardStatus){
    					  var opid = {operatorID:getOperator()};
    						var cs = Platform.ca.GetOperatorChildStatus(opid);
    						console.log("cs.isChild = " + cs.isChild); 
    						if(cs.isChild){
    								if (cs.isCanFeed) {
    										pCardDlg.HideDlg();
    										console.log("child card insert ,begin write child");
    										WriteChild();
    								}
    								else {
    										var string = "ParentChildCard_feeding :: It is not time to feeding";
    										pCardDlg.registerCallback( endFeeding,string );
    								}
    						}
    						else{
    								var string = "NSTV CA screen :: Feeding failed";
    								pCardDlg.registerCallback( endFeeding,string );												
    						}	
    				}        	
        }
        
				var ReadParent = function() {
								console.log("ReadParent reading......");
								var opid = {operatorID:getOperator()};
								var ret = Platform.ca.ReadParent(opid);
								var string1 = "NSTV CA screen :: Please insert the child-card";
								var string2 = "NSTV CA screen :: Invalid parent-card";
								console.log("ret.retVal = "+ ret.retVal);								
								if (ret.retVal == 0) {
										window.Platform.ca.setNotifySmcStatusCallback(function(){});
										pCardDlg.registerCallback( confirmChildCardInsert,string1 );
										window.AppManager.enter( "confirm PopDlg" ) ;																																								
								}
								else {
										pCardDlg.registerCallback( endFeeding,string2 );
										window.AppManager.enter( "confirm PopDlg" ) ;																												
								}
				}
						
				var WriteChild = function() {
    				window.AppManager.back();						
						console.log("enter write child begin write ...");
						var opid = {operatorID:getOperator()};
						var cs = Platform.ca.GetOperatorChildStatus(opid);												
						var ret = Platform.ca.WriteChild(opid);
						var string1 = "NSTV CA screen :: Feeding successful";
						var string2 = "NSTV CA screen :: Feeding failed";						
						console.log("ret.retVal= "  +ret.retVal);
						if (ret.retVal == 0) {
								pCardDlg.registerCallback( endFeeding,string1 );
								window.AppManager.enter("confirm PopDlg" ) ;	
						}
						else {
								pCardDlg.registerCallback( endFeeding,string2 );
								window.AppManager.enter("confirm PopDlg" ) ;										
						}			
				}
				var endFeeding = function(){					
						setTimeout(function(){pCardDlg.HideDlg();},2000);
        		window.AppManager.back();	
        		window.Platform.ca.setNotifySmcStatusCallback(handleSmcEvent);
        		getCAInfoDataPCCard();
        		showType('ParentChildCard');					
				};

				var confirmParentCardInsert = function(){
    				var smc = Platform.ca.GetCardStatus();
    				if(smc.cardStatus){
    						console.log("parent card is inserted");
								pCardDlg.HideDlg();
								window.AppManager.back();
								ReadParent();
    				}					
				}
				var isNotCanFeedDlg = new window.popDlg();
				isNotCanFeedDlg.$root.appendTo(self.$root);
				isNotCanFeedDlg.hide();
								
        self.onKey = function() {
            return true;
        };

        self.onShow = function() {
            updateScreen();
        };

        self.onKeyUP = function() {

            if (select_view) {
                return;
            }
            console.log("focusList.length = " + focusList.length);
            focusList.selectPrev();

        };

        self.onKeyDOWN = function() {
            if (select_view) {
                return;
            }
            focusList.selectNext();
        };
				self.onKeyCHANDOWN = function( ) {
						entitlementlist.getPagePrev();
						slotlist.getPagePrev();
						aclist.getPagePrev();
						ippvlist.getPagePrev();
						parentchildcardlist.getPagePrev();
						disentitlelist.getPagePrev();
						
						var typeName = cainfolist.getItemByIndex(1).select_view.getSelected().value;
						setType(typeName);
						var currentType = getType();
						if(currentType == 'Disentitle'){
							disentitlelist.getSelected().$root.removeClass("selected");
							removeFocusList();
							addSelectViewFocusList();
							addFocusList();
							focusList.updateItems(listData);
							focusList.select(2);
						}
				};
		
				self.onKeyCHANUP = function( ) {
						entitlementlist.getPageNext();
						slotlist.getPageNext();
						aclist.getPageNext();
						ippvlist.getPageNext();
						parentchildcardlist.getPageNext();
						disentitlelist.getPageNext();
						
						var typeName = cainfolist.getItemByIndex(1).select_view.getSelected().value;
						setType(typeName);
						var currentType = getType();
						if(currentType == 'Disentitle'){
							disentitlelist.getSelected().$root.removeClass("selected");
							removeFocusList();
							addSelectViewFocusList();
							addFocusList();
							focusList.updateItems(listData);
							focusList.select(2);
						}
				}; 
        self.onKeyRED = function() {
            if (disentitlelist.getSelected().$root.hasClass('selected')) {
                console.log("red button is used to delete the selected line");
            }
        };
        self.onKeyGREEN = function() {
            if (disentitlelist.getSelected().$root.hasClass('selected')) {
                console.log("green button is used to delete all ");
            }
        };
        
        self.onKeyYELLOW = function(){
            var opid = {operatorID:getOperator()};
            var cs = Platform.ca.GetOperatorChildStatus(opid);	        		
        		if(cs.isChild == 1){
        				if(cs.isCanFeed){
		        				window.Platform.ca.setNotifySmcStatusCallback(function(){});
		        				console.log("aaaaaaaaaaaaaaaaaaaaaaa");
										var string = "NSTV CA screen :: Please inserted parent card";								
										pCardDlg.registerCallback( confirmParentCardInsert,string );
										window.AppManager.enter( "confirm PopDlg" ) ;
								}else{
										isNotCanFeedDlg.show();
										isNotCanFeedDlg.showDlgMsg(TR('ParentChildCard_feeding :: It is not time to feeding'));
										setTimeout(function(){isNotCanFeedDlg.hide();},2000);	
								}        					
        		}
        		else{
        				ReadParent();
        		}
        		return ;	
        };
        self.onKeyOK = function() {
            var selectedindex = focusList.getSeletedIndex();
            switch (selectedindex) {
            case 0:
            case 1:
                {
                    if (!select_view) {
                        select_view = cainfolist.getItemByIndex(selectedindex).select_view;
                        select_view.activate(true);
                    } else {
                        select_view.activate(false);
                        select_view = null;
                        focusList.updateItems(listData);
                    }
                    break;
                }
            }
        };

				self.onKeyBACK = function() {
            window.Platform.ca.setNotifySmcStatusCallback(function(st){});
            window.AppManager.back();
            removeFocusList();
            if (select_view){
            		select_view.activate(false);
            		select_view = null;
          	}
        };
        
        self.onKeyLEFT = function() {
            if (!select_view) {
            		window.Platform.ca.setNotifySmcStatusCallback(function(st){});
                window.AppManager.back();
                removeFocusList();
                select_view = null;
                return;
            }
            select_view.selectPrev();
        };

        self.onKeyRIGHT = function() {
            if (!select_view) {
                return;
            }
            select_view.selectNext();
        };

        return self;
    };

    window.AppManager.addScene(name, CAinformations);
})();