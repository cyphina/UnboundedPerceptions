function setHealth(number = 0){
  let player = document.querySelector('.player:last-child');
  let hpBar = player.querySelector('.info .hp .progressbar .bar');
  let hpText = player.querySelector('.info .hp > span');
  if(hpBar){
    hpBar.style.width = number + '%';
  }
  
  if(hpText){
    hpText.innerText = `${number}/100`
  }
}

ue.interface.setHealth = setHealth;