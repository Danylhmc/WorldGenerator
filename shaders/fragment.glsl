#version 330 core 

out vec4 FragColor;

in vec3 FragPos;
in vec3 normal;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main(){
  vec3 up = vec3(0.0, 1.0, 0.0);
  float slope = 1 - max(dot(normalize(normal), up), 0.0);

  vec3 grassColor = vec3(0.18, 0.42, 0.12);
  vec3 dirtColor  = vec3(0.35, 0.24, 0.13);
  vec3 rockColor  = vec3(0.38, 0.37, 0.34);

  float grassToDirt = smoothstep(0.15, 0.35, slope);
  float dirtToRock  = smoothstep(0.35, 0.65, slope);

  vec3 groundColor = mix(grassColor, dirtColor, grassToDirt);

  vec3 terrainColor = mix(groundColor, rockColor, dirtToRock);

  //ambiant
  float ambiantStrength = 0.7f;
  vec3 ambiant = ambiantStrength * lightColor;
  
  //diffuse
  vec3 norm = normalize(normal);
  vec3 lightDir = normalize(lightPos - FragPos);
  float diff = max(dot(lightDir, normal), 0.0f);
  vec3 diffuse = diff * lightColor;

  //specular
  float specularStrength = 0.3f;
  float shininess = 32.0f;
  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 reflectDir = normalize(reflect(-lightDir, normal));
  float spec = pow(max(dot(reflectDir, viewDir), 0.0), shininess);
  vec3 specular = spec * lightColor * specularStrength;
  
  vec3 result = (ambiant + diffuse) * terrainColor;
  FragColor = vec4(result, 1.0);
}
