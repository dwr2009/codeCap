"use strict";
(function( ){

		var name = "Email";
		
		var Email = new window.Class(
		
		//parent
		window.ListViewDelegate,
		
		//constructor
		function (emailinfo){
            ListViewDelegate.call(this,name);
            this.$root.addClass('email_listlable');	
						console.log("emailinfo.status ="+ emailinfo.status);	
						if(emailinfo.status){
								var $email_emailid = $('<div>').addClass('column_fifteen ').appendTo(this.$root);
								var	$email_emailidicon = $('<div>').addClass('emailicon_unread ').appendTo($email_emailid);
								//var $email_emailidtext = $('<div>').addClass('emailtext ').text(emailinfo.email_emailid).appendTo($email_emailid);
							}
						else{
								var $email_emailid = $('<div>').addClass('column_fifteen ').appendTo(this.$root);
								var	$email_emailidicon = $('<div>').addClass('emailicon_readed').appendTo($email_emailid);
								//var $email_emailidtext = $('<div>').addClass('emailtext ').text(emailinfo.email_emailid).appendTo($email_emailid);
							}	
						this.iconName = $email_emailidicon;	
						this.emailId = emailinfo.id;
						this.status = emailinfo.status;	
						var $email_title = $('<div>').addClass('column_sixty').text(emailinfo.title).appendTo(this.$root);
						//var $email_importance = $('<div>').addClass('column_fifteen').text(emailinfo.importance).appendTo(this.$root);														
						if ( emailinfo.time instanceof Date ){
						var date_string = window.Format.formatDate(emailinfo.time)+' '+ window.Format.formatTime(emailinfo.time,'addsecond');
						var $email_createtime = $('<div>').addClass('column_four').text(date_string).appendTo(this.$root);
						}else{
						var $email_createtime = $('<div>').addClass('column_four').appendTo(this.$root);
						}						
						//var $email_title = $('<div>').addClass('column_sixty').text(emailinfo.title).appendTo(this.$root);			
						this.title = emailinfo.title;
						this.importance = emailinfo.importance;
						this.time = date_string; 
    	},
		//prototype
		{
		  getStatus: function(){
		  		return this.status;
		  	},
		  getIconName: function(){
		  		return this.iconName;
		  	},
		  getEmailId:function(){
		  		return this.emailId;
		  },
		  getEmailImportance:function(){
		  	return this.importance;
		  },
		  getEmailTime:function(){
		  	return this.time;
		  },
		  getEmailTitle:function(){
		  	return this.title;
		  },
	  }
		);
		
	var Emails = function(){
			var parent = new window.MainMenuScreen( name );
			var self = Object.create(parent);
			self.$root.attr('id','email');
			
			var $email = $('<div>').appendTo(self.$root);
			var $email_caption = $('<div>').addClass('caption').appendTo($email);
				
			var $email_emailidname = $('<div>').addClass('column_fifteen').append($TR('email_emailidname :: emailidname')).appendTo($email_caption);
			//var $email_importancename = $('<div>').addClass('column_fifteen ').append($TR('email_importancename :: importancename')).appendTo($email_caption);
			var $email_titlename = $('<div>').addClass('column_sixty ').append($TR('email_titlename :: titlename')).appendTo($email_caption);
			var $email_createtimename = $('<div>').addClass('column_four').append($TR('email_createtimename :: createtimename')).appendTo($email_caption);
					
			var list = new window.pageTurningListView( Email ) ;
			list.$root.addClass('listlable').appendTo( self.$root ) ;			
			
			var $emailBox = $( '<div>' ).addClass( 'emailBox' ).appendTo( $email ) ;
			var $emailBox_firstrow = $('<div>').addClass('emailBoxrow').appendTo($emailBox);
			var $emailBox_titlename = $( '<div>' ).addClass( 'emailBox_titlename' ).append( $TR( 'emailBox :: title' ) ).appendTo( $emailBox_firstrow ) ;
			var $emailBox_title = $( '<div>' ).addClass( 'emailBox_title' ).appendTo( $emailBox_firstrow ) ;
			
			var $emailBox_secondrow = $('<div>').addClass('emailBoxrow').appendTo($emailBox);
			var $emailBox_createtimename = $('<div>').addClass('emailBox_createtimename').append($TR('emailBox_createtime :: createtime')).appendTo($emailBox_secondrow);
			var $emailBox_createtime = $('<div>').addClass('emailBox_createtime').appendTo($emailBox_secondrow);
			
			var $emailBox_thirdrow = $('<div>').addClass('emailBoxrow').appendTo($emailBox);
			var $emailBox_importancename = $('<div>').addClass('emailBox_importancename').append($TR('emailBox_importance :: importance')).appendTo($emailBox_thirdrow);
			var $emailBox_importance = $('<div>').addClass('emailBox_importance').appendTo($emailBox_thirdrow);
			
			var $emailBox_forthrow = $('<div>').addClass('emailBoxrow').appendTo($emailBox);
			var $emailBox_contentname = $( '<div>' ).addClass( 'emailBox_contentname' ).append($TR('emailBox_content :: content')).appendTo( $emailBox_forthrow ) ;
			var $emailBox_content = $('<div>').addClass('emailBox_content').appendTo($emailBox_forthrow);

			var $email_last_lable = $('<div>').addClass('remindlable').appendTo(self.$root);
			var $email_redwarning = $('<div>').addClass('redwarning').appendTo($email_last_lable);
			var $email_redwarningtext = $('<div>').addClass('redwarningtext').append($TR('email_delete :: delete')).appendTo($email_last_lable);
			var $email_green_warning = $('<div>').addClass('greenwarning').appendTo($email_last_lable);
			var $email_green_warningtext = $('<div>').addClass('greenwarningtext').append($TR('email_delete :: deleteall')).appendTo($email_last_lable);
		
			var max_email = 100;
			var emailHeads = null;			
			var flag = 0; 
																		
			var getEmailInfo = function(){
				var currentLang = Platform.settings.getLanguage();
				var req = {fromIndex:0, count:max_email, language:currentLang.toUpperCase()};
				//var req = {fromIndex:0, count:max_email};
				emailHeads = Platform.ca.GetEmailHeads(req);
				var	total_email = emailHeads.emailHeadList.length;						
				var q = new Array(); 
				for(var i = 0; i < total_email; i++){
						var dt = new Date();
						dt.setTime(emailHeads.emailHeadList[i].createTime*1000);
						var p ={
							status:emailHeads.emailHeadList[i].newEmail,
							id:emailHeads.emailHeadList[i].actionID,
							importance:emailHeads.emailHeadList[i].importance ? (TR('email :: Important')) : (TR('email :: Normal')),
							time:dt,
							title:emailHeads.emailHeadList[i].emailHead,							
						}; 
						q.push(p);
				}
				console.log("q.length = "+ q.length);
				q.sort(function(a,b){return b.time-a.time});
				return q;				
			}
			/*var getEmailSpaceInfo = function(){
				console.log("aaaaaaaaaaaaaaaaaaaaaa");
				var ret = Platform.ca.GetEmailSpaceInfo();
				console.log("bbbbbbbbbbbbbbbbbbbbbbbb");
				return ret.emptyNum;	
			}*/
			var getEmailContent = function(emailid){
		 		console.log("-> Platform.ca.getEmailContent") ;
				var eid = {emailID:emailid};
				var content = Platform.ca.GetEmailContent(eid);	 		
		 		return content.emailContent;
		 	}
		 	var DeleteEmail = function(emailid){
				 	var item = {emailID:emailid};
					Platform.ca.DelEmail(item);
					var req = {fromIndex:0, count:max_email};
					emailHeads = Platform.ca.GetEmailHeads(req);
					console.log("delete email successful");
		 	}
		 	var DeleteAllEmail = function(){
					for (var i=0; i<emailHeads.emailHeadList.length; i++) {
							var item = {emailID:emailHeads.emailHeadList[i].actionID};
							Platform.ca.DelEmail(item);
					}	
					var req = {fromIndex:0, count:max_email};
					emailHeads = Platform.ca.GetEmailHeads(req);	 	
		 	}
		 	
			var updateScreen = function( ) {       			
          		var emailinfo = getEmailInfo();
          		list.updateItems(emailinfo) ;
         		list.getPageCountInit();
				list.show( ) ;
				$emailBox.hide( ) ;
				//$email_totalemails.text(TR('email_totalemails :: totalemails')+list.length);
				newMails = traverse(getEmailInfo());
				//$email_newmails.text(TR('email_newemails :: newemails')+newMails);
				//var count = getEmailSpaceInfo() ;
				//$email_freespace.text(TR('email_emptyemails :: emptyemails') + count);	
 	
			} ;

			var updateEmailBoxScreen = function(item){				
				$emailBox_title.text(item.getEmailTitle());
				console.log("item.getEmailTitle() =============" + item.getEmailTitle() ) ;
				$emailBox_createtime.text(item.getEmailTime());
				$emailBox_importance.text(item.getEmailImportance());
				$emailBox_content.html( getEmailContent(item.getEmailId()).replace(/\n/g, "<br/>"));
					
			}			
					
			function traverse(list){
				console.log("list.length = "+ list.length);	
				var new_flag = 0;				
				for(var i = 0 ; i < list.length ;i++){
						var item = list[i];
						if(item.status)
						{
							new_flag++;
						}
				}
				return new_flag;					
			}

			var updateMails = function (item){
					var newTotalMails = getEmailInfo();
					newMails = traverse(newTotalMails);
					//$email_totalemails.text(TR('email_totalemails :: totalemails')+newTotalMails.length);
					//$email_newmails.text(TR('email_newemails :: newemails')+newMails);
					console.log("item.getStatus() = "+item.getStatus());
					item.getIconName().removeClass('emailicon_unread');
					item.getIconName().addClass('emailicon_readed');
			}					
			var totalMails = getEmailInfo();			
			var newMails = traverse(totalMails);
			//var freespace = getEmailSpaceInfo();
									

			var $email_last_lable = $('<div>').addClass('email_last_lable').appendTo(self.$root);
			
			//var	$email_newmails = $('<div>').addClass('email_newmails').text(newMails + "/" + totalMails.length).appendTo($email_last_lable);
			//var $email_totalemails = $('<div>').addClass('column_half').text(totalMails.length).appendTo($email_last_lable);
			
			var deleteEmailDlg = new window.popDlg();
			deleteEmailDlg.$root.addClass('deleteEmailDlg').appendTo($email);
			
			self.onKey = function( ) {
					return true ;
			};
			self.onKeyOK = 
			self.onKeyRIGHT = function(){
					var item = list.getSelected();	
				  updateEmailBoxScreen(item);
				  $emailBox.show();
				  updateMails(item);	
				  flag = 1;
			};
			self.onKeyBACK = 
			self.onKeyLEFT = function(){
					if(flag == 1){
						$emailBox.hide();
						flag = 0;		
					}
					else{
					window.AppManager.back();	
					}	
			};	
			self.onShow = function(){	
					updateScreen( ); 
			};
			self.onKeyUP = function( ) {
				if(flag == 0){
					list.selectPrev( ) ;
				}		
			};
			self.onKeyDOWN = function( ) {
				if(flag == 0){
					list.selectNext( ) ;
				}			
			};

			self.onKeyRED = function(){
					if(list.length > 0){
							var callbackOk = function ( ) {
									var item = list.getSelected();
									DeleteEmail(item.getEmailId());
									window.AppManager.back() ;							
									selectPopDlg.HideSelectPopDlg();																													
									updateScreen();	
									deleteEmailDlg.show();																					
									deleteEmailDlg.showDlgMsg(TR('email_Email deleted successfully'));
									setTimeout(function(){deleteEmailDlg.hide()},2000);																																					
							}
							var callbackCancel = function ( ) {
								window.AppManager.back() ;					
							}
							var string = "email_Are you sure to delete this email";
							var selectPopDlg = window.AppManager.getWidget("Select PopDlg");
							selectPopDlg.registerCallback( callbackOk,callbackCancel,string );
							window.AppManager.enter( "Select PopDlg" ) ;
				  }

			};
			self.onKeyGREEN = function(){
					if(list.length > 0){
							var callbackOk = function ( ) {
								DeleteAllEmail();
								selectPopDlg.HideSelectPopDlg();												
								window.AppManager.back() ;						
								updateScreen();
								deleteEmailDlg.show();
								deleteEmailDlg.showDlgMsg(TR('email_Email deleted successfully'));
								setTimeout(function(){deleteEmailDlg.hide()},2000);						
							}
							var callbackCancel = function ( ) {
								window.AppManager.back() ;
							}
							var string = "email_Are you sure to delete all emails";
							var selectPopDlg = window.AppManager.getWidget("Select PopDlg");
							selectPopDlg.registerCallback( callbackOk, callbackCancel,string );
							window.AppManager.enter( "Select PopDlg" ) ;
				  }
			};	
			self.onKeyCHANDOWN = function( ) {
					list.getPagePrev( ) ;				
			};
			self.onKeyCHANUP = function( ) {
					list.getPageNext( ) ;				
			};		
				
		return self ;
	} ;
		
			
			window.AppManager.addScene( name,Emails ) ;
})();
