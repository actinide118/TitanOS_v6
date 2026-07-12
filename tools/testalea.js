a=1664525
c=1013904223
m=0xFFFFFFFF

seed=131
function next(){
  seed = (a * (seed) + c) % m
}

for(let i=0;i<100;i++){
  next()
  console.log(seed)
}
