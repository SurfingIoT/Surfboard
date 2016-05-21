// Assim que entrar no site...
$(document).ready(function () {

  // scroll suave, quando houver hash no link
  if (window.location.hash) {
    //pego a hash
    var hash = window.location.hash;
    //animo até o elemento de id equivalente da hash
    $('html, body').animate({scrollTop: $(hash).offset().top}, 2000);
  }

  // para rolar entre secoes
  function goToByScroll(section) {
    $('html, body').animate({scrollTop: $(section).offset().top - 40}, 1000);
  }
  $("#navOne li a").click(function (e) {
    e.preventDefault();
    goToByScroll($(this).attr("href"));
  });
  $("#navTwo li a").click(function (e) {
    e.preventDefault();
    goToByScroll($(this).attr("href"));
  });
  $(".scroll-page").click(function (e) {
    e.preventDefault();
    goToByScroll($(this).attr("href"));
  });
  // para rolar entre secoes

});

// Depois que a page ja tenha sido carregada...
$(window).load(function () {

  /* SCROLL */

  // para rolar ao topo
  $('a[href=#top]').click(function () {
    $('html, body').animate({scrollTop: 0}, 1000);
    return false;
  });
  // para rolar ao topo
  // 
  // 
  // exibir botao de rolagem depois de rolar page para baixo
  $(window).scroll(function () {
    if ($(this).scrollTop() > 120) {
      $('.btn-up').removeClass('hidden').delay('slow');
      $('.btn-up').css('opacity', '1').fadeIn();
    } else {
      $('.btn-up').css('opacity', '0').fadeIn();
      $('.btn-up').addClass('hidden').delay('slow');
    }
  });
  // exibir botao de rolagem depois de rolar page para baixo

  /* SCROLL */


  /* FUNCOES DESKTOP */

  function paraDesktop() {
    if ($(window).width() >= 768) {
      // exibir navbar depois de rolar page para baixo
      $(window).scroll(function () {
        if ($(this).scrollTop() > 60) {
          $('.navbar').removeClass('hidden');
        } else {
          $('.navbar').addClass('hidden');
        }
        if ($(this).scrollTop() > 60) {
          $('.navbar').removeClass('hidden').delay('slow');
          $('.navbar').css('opacity', '1').fadeIn();
        } else {
          $('.navbar').css('opacity', '0').fadeIn();
          $('.navbar').addClass('hidden').delay('slow');
        }
        if ($(this).scrollTop() > 120) {
          $('.btn-up').removeClass('hidden').delay('slow');
          $('.btn-up').css('opacity', '1').fadeIn();
        } else {
          $('.btn-up').css('opacity', '0').fadeIn();
          $('.btn-up').addClass('hidden').delay('slow');
        }
      });

      $('.tip-top').tooltip({
        container: 'body',
        placement: 'top'
      });
      $('.tip-right').tooltip({
        container: 'body',
        placement: 'right'
      });
      $('.tip-bottom').tooltip({
        container: 'body',
        placement: 'bottom'
      });
      $('.tip-left').tooltip({
        container: 'body',
        placement: 'left'
      });
    }
    if ($(window).width() < 768 || $(window).scrollTop() > 120) {
      $('.navbar').removeClass('hidden');
    }
  }
  paraDesktop();
  $(window).resize(paraDesktop);

  /* FUNCOES DESKTOP */



  /* CENTRALIZAR BOTAO */
  function centralizarBotaoSubir() {
    var w = $(window).width();
    w = (w / 2) - 38;
    $('.btn-up').css('left', w + 'px');
  }
  centralizarBotaoSubir();
  $(window).resize(centralizarBotaoSubir);
  /* CENTRALIZAR BOTAO */


});


// mobile: ao tocar em alguma opcao do menu, 
// a page rola ate a secao e esconde o menu
function close_toggle() {
  if ($(window).width() <= 768) {
    $('.menu a').on('click', function () {
      $(".navbar-toggle").click();
    });
  }
  else {
    $('.nav a').off('click');
  }
}
close_toggle();
$(window).resize(close_toggle);

// AddThis Smart Layers API
function inicializarAddThis() {

  addthis.layers({
    'domain': 'www.globalcode.com.br',
    'linkFilter': function (link, layer) {
      console.log(link.title + ' - ' + link.url + " - " + layer);
      return link;
    },
    'mobile': false,
    'share': {
      'theme': 'light',
      'position': 'right',
      'offset': {
        'top': '35%'
      },
      'services': 'facebook,twitter,linkedin,email,compact',
      'postShareTitle': 'Obrigado por compartilhar!',
      'postShareFollowMsg': 'Siga a Globalcode nas redes sociais também'
    }
  });

}