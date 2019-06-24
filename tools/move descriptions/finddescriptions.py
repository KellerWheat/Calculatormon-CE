from lxml import html
import requests
page = requests.get('https://pokemondb.net/move/generation/1')
tree = html.fromstring(page.content)


inputlinks = tree.xpath('//a[@class="ent-name"]')

links = []

for link in inputlinks:
    links.append('https://pokemondb.net' + link.attrib['href'])

descriptions = []

for link in links:
    page = requests.get(link)
    tree = html.fromstring(page.content)
    descriptions.append(tree.xpath('//span[@class="igame firered"]/parent::*/parent::*/td/text()')[0][:-1])
    
print(str(descriptions).replace("'", "\"").replace("’", "\'").replace("[", "{").replace("]", "}"))
