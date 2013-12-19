$(function(){ 
    $("#startbtn").click(function(){ 
	$.ajax({ 
            type: 'POST', 
            url: '/rotate/', 
            dataType: 'json', 
            cache: false, 
            error: function(){ 
		alert('出错了！'); 
		return false; 
            }, 
            success:function(json){ 
		$("#startbtn").unbind('click').css("cursor","default"); 
		var a = json.angle; //角度 
		var p = json.prize; //奖项 
		$("#startbtn").rotate({ 
                    duration:3000, //转动时间 
                    angle: 0, 
                    animateTo:1800+a, //转动角度 
                    easing: $.easing.easeOutSine, 
                    callback: function(){ 
			var con = confirm(''+p+'\n请在专柜出示本页面，登记兑奖'); 
			return con;
                    } 
		}); 
            } 
	}); 
    }); 
});
