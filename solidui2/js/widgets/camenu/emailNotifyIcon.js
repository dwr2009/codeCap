var emailNotifyIcon = new window.Class(

	// parent
	window.Widget,

	// constructor
	function() {
		Widget.call( this,"Email Icon ") ;

		this.interval = 0;
		this.$newEmail = $( '<img src = ./templates/hkc/images/mail_unread_big.png  id = img_new style = display:none >' ).addClass('newEmail').appendTo( this.$root ) ;

	},

	// prototype
	{
		EmailNotifyIcon:function(param){
				console.log("iconStatus = " + param.showFlag);

				if (param.showFlag) {
					var currentLang = Platform.settings.getLanguage();
					var req = {fromIndex:0, count:100, language:currentLang.toUpperCase()};
					var emailHeads = Platform.ca.GetEmailHeads(req);
					if (!emailHeads.newEmail) {
						param.showFlag = 0;
					}
				}

				if (param.showFlag == 2) {
					this.interval = setInterval(function(){
						$("#img_new").toggle();
					}, 1000);
				}
				else {
					if (param.showFlag) {    					
						$("#img_new").show();
					}
					else {
						$("#img_new").hide();
					}
					if (this.interval) {
						clearInterval(this.interval);
						this.interval = 0;
					}	
				}
		},
	}

) ;
