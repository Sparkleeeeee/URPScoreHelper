<script type="text/javascript">
	function check_password() {
		var r1 = document.getElementById("i_xhs").value;
		if(r1=="")
		{
			$.toast("(��o��) �ܵ�����Щʲô�ɣ�");
			return false;
		}
		if(r1.length > 12)
		{
			$.toast("(��o��) ����̫������ϵͳ�ǲ�ס");
			return false;
		}
		var patrn=/^(\w){6,12}$/;
		if (!patrn.exec(r1)){
			$.toast("ֻ������6-12����ĸ�����֡��»���");
			return false;
		}
		if(!document.getElementById("i_chk").checked){
			$.toast("�����ұ߿�����ȷ����������:-)");
			return false;
		}
		$.showPreloader('O(��_��)O �����޸�����');
		setTimeout(function () {
			$.hidePreloader();
		}, 10000);
		return true;
	}
</script>
<div class="page-group">
	<div class="page">
		<header class="bar bar-nav">
		<button class="button button-link button-nav pull-left"><span class="icon icon-left"></span><a href="main.cgi">����</a></button>
		<button class="button button-link button-nav pull-right"><a href="Comments.html">�²�</a>&nbsp;&nbsp;<span class="icon icon-message"></span></button>
		<h1 class="title">���������޸�</h1>
		</header>
		<nav class="bar bar-tab">
		<a class="tab-item external active" href="main.cgi">
		<span class="icon icon-star"></span>
		<span class="tab-label">���������޸�</span>
		</a>
		</nav>
		<div class="content">
			<div class="content-block" style="overflow: hidden">
				<p>
					<b>ʹ����֪</b><small> | Xiu Xiu is more healthy.</small>
				</p>
				<p><small><b>ΪʲôҪ�޸����룿</b>������֪������ϵͳ���õ�Ĭ���������ѧ�ţ���������Ϣ���Ӵ����˾��ѵĻ��ᡣһ�����ѧ������ѧ�����ݣ��������ӿ��Ի��ѧ����һ����Ϣ��������ѧ�š�֤���ա�רҵ�����֤����ͥסַ�ȣ�������թ�����������кڿͻ��Ѽ���Щ��Ϣ¼���繤�⣬�Ǻ�����᲻�����롣<b>���ڹ���ѧԺ</b>��ֱ�ӽ�����ϵͳ��¶�ڹ����������ݰ�ȫ���᧿�Σ�����⣬����ѧԺ�ᶨ������ѧ�����롣���ԣ�ÿ���޸�һ�ν��������Ǳ�Ҫ�ġ���Ҳ��ΪʲôС�����ڹ��������HTTPS���ܵ�ԭ��֮һ��</small></p>
				<p><small><b>���棺</b>�㽫Ҫ�޸����룬����ϸ�˶�������Ϣ���޸ĺ��������������Ʊ��档����������޸ĺ�����룬���ǽ�����Ϊ������ʱֻ������ѧУ�����š����ǲ��е��κη������Ρ��������Ƴ���Ϊ12λ���޸ĳɹ�����Ҫ���µ�¼��</small></p>
				<canvas id="i_canvas" style="position:absolute;top:0;left:0;right:0;bottom:0"></canvas>
				<form action="changePassword.cgi" method="post">
					<div class="list-block">
						<ul style="opacity: 0.75;z-index:9999">
							<li>
								<div class="item-content">
									<div class="item-media">
										<i class="icon icon-form-name"></i>
									</div>
									<div class="item-inner">
										<div class="item-title label">
											������
										</div>
										<div class="item-input">
											<input name="mm" type="password" placeholder="�12λ" id="i_xhs">
										</div>
									</div>
								</div>
							</li>
							<li>
								<div class="item-content">
									<div class="item-media">
										<i class="icon icon-form-name"></i>
									</div>
									<div class="item-inner">
										<div class="item-title label">
											ȷ����������
										</div>
										<label class="label-switch">
												<input type="checkbox" id="i_chk">
												<div class="checkbox"></div>
										</label>
									</div>
								</div>
							</li>
						</ul>
					</div>
					<div class="content-block">
						<div class="row">
							<div class="col-100">
								<input onclick="return check_password();" style="z-index:9999" type="submit" value="ȷ���޸�" class="button button-big button-fill button-success" />
							</div>
							<div class="col-100">
								<a style="z-index:9999;margin-top:10px" title="&lt; �����ˣ���ȥ" class="button button-big button-fill external" href="main.cgi">&lt; �����ˣ���ȥ</a>
							</div>
						</div>
					</div>
				</div>
			</form>
			<center><p id="login_query">��&nbsp;&nbsp;<big>%ld</big>&nbsp;&nbsp;λͬѧ������&nbsp;&nbsp;<big>%d</big>&nbsp;&nbsp;�β�ѯ</p></center>
		</div>
	</div>
</div>