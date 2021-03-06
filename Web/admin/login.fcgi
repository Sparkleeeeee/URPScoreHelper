<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no" />
    <title>管理员登录 - %s</title>  
    <link rel="stylesheet" href="css/pintuer.css">
	<link rel="stylesheet" href="../css/font-awesome.min.css">
    <link rel="stylesheet" href="css/admin.css">
	<link rel="shortcut icon" href="../URPScoreHelper.ico">
    <script src="../js/jquery.min.js"></script>
    <script src="js/pintuer.js"></script>
	<script type="text/javascript">if(self!=top){top.location.href=self.location.href;}</script>
</head>
<body>
<div class="bg"></div>
<div class="container">
    <div class="line bouncein">
        <div class="xs6 xm4 xs3-move xm4-move">
            <div style="height:50px;"></div>
            <div class="media media-y margin-big-bottom">           
            </div>         
            <form action="login.fcgi" method="post">
            <div class="panel loginbox">
                <div class="text-center margin-big padding-big-top">
					  <div class="logo margin-big-left fadein-top" style="margin-bottom:20px">
						<h1 style="text-shadow:0 0 0 #000, 0px 0px 1px #000"><img src="../URPScoreHelper.ico" class="radius-circle rotate-hover" height="50" alt="" />%s</h1>
				      </div>
				</div>
                <div class="panel-body" style="padding:30px; padding-bottom:10px; padding-top:10px;">
                    <div class="form-group">
                        <div class="field field-icon-right">
                            <input autofocus type="text" class="input input-big" name="usr" placeholder="请输入管理员帐号" data-validate="required:请填写帐号" />
                        </div>
                    </div>
                    <div class="form-group">
                        <div class="field field-icon-right">
                            <input type="password" class="input input-big" name="pwd" placeholder="请输入登录密码" data-validate="required:请填写密码" />
                        </div>
                    </div>
                </div>
                <div style="padding:0px 30px 10px 30px">
					<button type="submit" class="button button-block bg-main text-big input-big"><i class="fa fa-unlock-alt"></i>登录</button>
					<a style="text-align:center;margin-top:5px;background-color:#39be0e;border-color:#39be0e" href="../" class="button button-block bg-main text-big input-big"><i class="fa fa-arrow-left"></i>前台首页</a>
				</div>
				<div style="text-align:center;margin:0 auto;font-weight:bold;color:#fff;text-shadow:0 0 0 #000, 0px 0px 1px #000;">
					<p style="margin-bottom:0px">%s</p>
					<p style="padding-bottom:10px">%s</p>
				</div>
            </div>
            </form>          
        </div>
    </div>
</div>
</body>
</html>